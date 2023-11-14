/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:45:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/14 12:41:15 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_smplcmd *smplcmd, t_shell *shell)
{
	if (smplcmd->builtin == ECHO)
		return (execute_echo(*smplcmd));
	if (smplcmd->builtin == CD)
		return (execute_cd(*smplcmd, shell));
	if (smplcmd->builtin == PWD)
		return (execute_pwd());
	if (smplcmd->builtin == EXPORT)
		return (execute_export(shell, *smplcmd));
	if (smplcmd->builtin == UNSET)
		return (execute_unset(*smplcmd, shell));
	if (smplcmd->builtin == ENV)
		return (execute_env(*smplcmd, *shell));
	if (smplcmd->builtin == EXIT)
		return (execute_exit(*smplcmd, shell));
	return (EXIT_FAILURE);
}

int	do_redirection(char *filename, int open_flags, int redirect_target)
{
	int	fd;

	fd = open(filename, open_flags, 0000644);
	if (fd == -1)
		return (perror(filename), EXIT_FAILURE);
	if (dup2(fd, redirect_target) == -1)
		return (perror("dup2"), close(fd), EXIT_FAILURE); // free also
	if (close(fd) == -1)
		return (perror("close"), EXIT_FAILURE); // free also
	return (EXIT_SUCCESS);
}

int	handle_redirection_per_type(t_redirection *redirection)
{
	if (redirection->type == 1 || redirection->type == 2)
		return (do_redirection(redirection->file, O_RDONLY, STDIN_FILENO));
	else if (redirection->type == 3)
		return (do_redirection(redirection->file, O_TRUNC | O_CREAT | O_RDWR, STDOUT_FILENO));
	else
		return (do_redirection(redirection->file, O_APPEND | O_CREAT | O_RDWR, STDOUT_FILENO));
}

int	handle_redirections(t_redirection *redirections)
{
	int	exit_redirection;

	while (redirections)
	{
		exit_redirection = handle_redirection_per_type(redirections);
		if (exit_redirection != EXIT_SUCCESS)
			return (exit_redirection);
		redirections = redirections->next;
	}
	return (EXIT_SUCCESS);
}

int	init_pipe(t_cmd *cmd)
{
	if (cmd->next)
	{
		if (pipe(cmd->pipe) == -1)
			return (perror("pipe"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	free_child_process(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*begin;

	begin = cmd;
	while (begin->prev)
		begin = begin->prev;
	free_structs(&begin);
	free_all_envs(&(shell->exported_vars));
	free_all_envs(&(shell->env));
	rl_clear_history();
	//TODO: Check if dupped savedSTDIN/STDOUt should also be closed?
}

void	handle_child_process(t_cmd *cmd, t_shell *shell)
{
	int	exit_redirections;
	int exit_code;

	init_signals(CHILD_MODE);
	if (close(shell->std_stdin)  == -1)
	{
		close(shell->std_stdout);
		if (cmd->prev)
			close(cmd->prev->pipe[0]);
		if (cmd->next)
			close(cmd->pipe[1]);
		free_child_process(cmd, shell);
		return exit(EXIT_FAILURE);
	}
	if (close(shell->std_stdout) == -1)
	{
		if (cmd->prev)
			close(cmd->prev->pipe[0]);
		if (cmd->next)
			close(cmd->pipe[1]);
		free_child_process(cmd, shell);
		return exit(EXIT_FAILURE);
	}
	if (cmd->prev)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(cmd->prev->pipe[0]);
			if (cmd->next)
				close(cmd->pipe[1]);
			free_child_process(cmd, shell);
			return exit(EXIT_FAILURE);
		}
		if (close(cmd->prev->pipe[0]) == -1)
		{
			perror("close");
			if (cmd->next)
				close(cmd->pipe[1]);
			free_child_process(cmd, shell);
			return exit(EXIT_FAILURE);
		}
	}
	if (cmd->next)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(cmd->pipe[1]);
			free_child_process(cmd, shell);
			return (exit(EXIT_FAILURE));
		}
		if (close(cmd->pipe[1]) == -1 || close(cmd->pipe[0]) == -1)
			return (perror("close"), free_child_process(cmd, shell), exit(EXIT_FAILURE));
	}
	exit_redirections = handle_redirections(cmd->smplcmd->redir);
	if (exit_redirections != EXIT_SUCCESS)
		return (free_child_process(cmd, shell), exit(exit_redirections));
	if (cmd->smplcmd->builtin == 0)
	{
		exit_code = exec_simple_command(cmd->smplcmd, shell);
		free_child_process(cmd, shell);
		exit(exit_code);
	}
	else
	{
		exit_code = exec_builtin(cmd->smplcmd, shell);
		free_child_process(cmd, shell);
		exit(exit_code);
	}
}

int	handle_parent_process(t_cmd *cmd)
{
	if (cmd->prev && close(cmd->prev->pipe[0]) == -1)
	{
		perror("close");
		if (cmd->next)
		{
			close(cmd->pipe[1]);
			close(cmd->pipe[0]);
		}
		return (EXIT_FAILURE);
	}
	if (cmd->next && close(cmd->pipe[1]) == -1)
	{
		return (perror("close"), close(cmd->pipe[0]), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_single_command(t_shell *shell, t_cmd *cmd)
{
	int	exit_redirections;

	exit_redirections = handle_redirections(cmd->smplcmd->redir);
	if (exit_redirections != EXIT_SUCCESS)
		return (exit_redirections);
	if (cmd->smplcmd->builtin == 0)
	{
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (perror("fork"), EXIT_FAILURE);
		if (cmd->pid == 0)
		{
			if (close(shell->std_stdin)  == -1)
			{
				close(shell->std_stdout);
				if (cmd->prev)
					close(cmd->prev->pipe[0]);
				if (cmd->next)
					close(cmd->pipe[1]);
				free_child_process(cmd, shell);
				exit(EXIT_FAILURE);
			}
			if (close(shell->std_stdout) == -1)
			{
				if (cmd->prev)
					close(cmd->prev->pipe[0]);
				if (cmd->next)
					close(cmd->pipe[1]);
				free_child_process(cmd, shell);
				exit(EXIT_FAILURE);
			}
			exit(exec_simple_command(cmd->smplcmd, shell));
		}
	}
	else
		shell->last_exit_code = exec_builtin(cmd->smplcmd, shell);
	return (EXIT_SUCCESS);
}

int	wait_all_commands_on_error(t_cmd *start, t_cmd *cmd)
{
	int		status;

	status = EXIT_FAILURE;
	while (start != cmd)
	{
		waitpid(start->pid, &status, 0);
		start = start->next;
	}
	return (EXIT_FAILURE);
}

int	handle_pipe_error(t_cmd *start, t_cmd *cmd)
{
	if (cmd->prev)
		close(cmd->prev->pipe[0]);
	return (wait_all_commands_on_error(start, cmd));
}

int	handle_fork_error(t_cmd *start, t_cmd *cmd)
{
	perror("fork");
	close(cmd->pipe[0]);
	close(cmd->pipe[1]);
	return (handle_pipe_error(start, cmd));
}

int	handle_multiple_commands(t_shell *shell, t_cmd *cmd)
{
	t_cmd	*start;

	start = cmd;
	while (cmd)
	{
		if (init_pipe(cmd) != EXIT_SUCCESS)
			return (handle_pipe_error(start, cmd));
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (handle_fork_error(start, cmd));
		else if (cmd->pid == 0)
			handle_child_process(cmd, shell);
		else
		{
			if (handle_parent_process(cmd) != EXIT_SUCCESS)
				return (wait_all_commands_on_error(start, cmd), EXIT_FAILURE);
		}
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

int	handle_waiting_processes(t_cmd *cmd, t_shell *shell)
{
	int		status;
	t_cmd	*temp;
	int		is_error;

	status = 0;
	temp = cmd;
	is_error = 0;
	while (temp)
	{
		if (waitpid(temp->pid, &status, 0) == -1)
		{
			perror("waitpid");
			is_error = 1;
		}
		status = WEXITSTATUS(status);
		temp = temp->next;
	}
	temp = cmd;
	while (temp)
	{
		if (temp->smplcmd->redir != NULL && temp->smplcmd->redir->type == 2
			&& temp->smplcmd->redir->to_delete == 1)
		{
			if (unlink(temp->smplcmd->redir->file) != 0)
			{
				perror("unlink");
				is_error = 1;
			}
		}
		temp = temp->next;
	}
	if (is_error)
		return (EXIT_FAILURE);
	shell->last_exit_code = status;
	return (EXIT_SUCCESS);
}

void	free_set_failure_unlink(t_cmd **cmds, t_shell *shell)
{
	t_cmd	*temp;

	temp = *cmds;
	while (temp)
	{
		if (temp->smplcmd->redir != NULL && temp->smplcmd->redir->type == 2
			&& temp->smplcmd->redir->to_delete == 1)
		{
			unlink(temp->smplcmd->redir->file);
		}
		temp = temp->next;
	}
	free_structs(cmds);
	shell->last_exit_code = EXIT_FAILURE;
	return ;
}

int	init_heredoc_execution(t_cmd *cmds, t_shell *shell)
{
	int		i;
	t_cmd	*temp;

	i = 0;
	temp = cmds;
	init_signals(HEREDOC_MODE);
	while (temp)
	{
		if (temp->smplcmd->redir != NULL && temp->smplcmd->redir->type == 2)
		{
			if (exec_heredoc(temp->smplcmd->redir, i, shell) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
			i++;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

void	exec_commands(char *cmd, t_shell *shell)
{
	t_cmd	*cmds;

	cmds = parse_commands(cmd, shell);
	if (!cmds)
		return ;
	if (init_heredoc_execution(cmds, shell) != EXIT_SUCCESS)
		return (free_set_failure_unlink(&cmds, shell));
	init_signals(GLOBAL_MODE);
	if (cmds->next)
	{
		if (handle_multiple_commands(shell, cmds) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	else
	{
		if (handle_single_command(shell, cmds) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	if (cmds->next || cmds->smplcmd->builtin == 0)
	{
		if (handle_waiting_processes(cmds, shell) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	free_structs(&cmds);
	return ;
}

char	*get_env_entry_str(t_vars *env)
{
	int		i;
	int		j;
	char	*env_entry;

	i = 0;
	j = 0;
	env_entry = ft_calloc(2 + ft_strlen(env->key) + ft_strlen(env->value), 1);
	if (!env_entry)
		return (NULL);
	while (env->key[j])
		env_entry[i++] = env->key[j++];
	env_entry[i++] = '=';
	j = 0;
	while (env->value[j])
		env_entry[i++] = env->value[j++];
	env_entry[i] = '\0';
	return (env_entry);
}

void	free_env_arr(char ***env_as_char_arr)
{
	int	i;

	i = 0;
	if (*env_as_char_arr)
	{
		while ((*env_as_char_arr)[i])
			free((*env_as_char_arr)[i++]);
		free((*env_as_char_arr));
	}
}

char	**get_env_as_char_arr(t_shell *shell)
{
	int		size;
	t_vars	*env;
	char	**env_as_char_arr;
	int		i;

	env = shell->env;
	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	env_as_char_arr = ft_calloc(size + 1, sizeof(char *));
	if (!env_as_char_arr)
		return (perror("Allocation failed"), NULL);
	env = shell->env;
	i = 0;
	while (env)
	{
		env_as_char_arr[i] = get_env_entry_str(env);
		if (!env_as_char_arr[i++])
			return (free_env_arr(&env_as_char_arr), NULL);
		env = env->next;
	}
	return (env_as_char_arr);
}

int	exec_simple_command(t_smplcmd *smplcmd, t_shell *shell)
{
	char	*path;
	char	**args;
	char	**env;

	args = smplcmd->args;
	path = smplcmd->path;
	init_signals(CHILD_MODE);
	if (access(path, X_OK) != 0 || ft_strlen(smplcmd->args[0]) == 0
		|| (ft_strncmp(smplcmd->args[0], ".", 2)) == 0)
	{
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	env = get_env_as_char_arr(shell);
	if (!env)
		return (EXIT_FAILURE); 
	if (execve(path, args, env) == -1)
		return (perror("execve"), free_env_arr(&env), 1);
	return (free_env_arr(&env), EXIT_FAILURE); 
}
