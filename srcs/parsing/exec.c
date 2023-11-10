/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:45:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 10:17:04 by ppfiel           ###   ########.fr       */
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
	return (1);
}

// void	exec_commands(char *cmd, t_shell *shell)
// {
// 	t_cmd	*cmds;
// 	t_cmd	*temp;
// 	int		fd[2];
// 	int		prev_fd = STDIN_FILENO;

// 	cmds = parse_commands(cmd, shell);
// 	if (!cmds)
// 		return ;
// 	temp = cmds;
// 	while (temp)
// 	{
// 		if (temp->next)
// 		{
// 			if (pipe(fd) == -1)
// 				(perror("pipe"), exit(EXIT_FAILURE)); //free also
// 		}
// 		pid_t pid = fork();
// 		if (pid == -1)
// 			(perror("fork"), exit(EXIT_FAILURE)); // free also	
// 		else if (pid == 0)
// 		{
// 			if (temp->next)
// 			{
// 				if (dup2(fd[1], STDOUT_FILENO) == -1)
// 					(perror("dup2"), exit(EXIT_FAILURE)); // free also
// 				if (close(fd[0]) == -1 || close(fd[1]) == -1)
// 					(perror("close"), exit(EXIT_FAILURE)); // free also
// 			}
// 			if (prev_fd != STDIN_FILENO)
// 			{
// 				if (dup2(prev_fd, STDIN_FILENO) == -1)
// 					(perror("dup2"), exit(EXIT_FAILURE)); // free also
// 				if (close(prev_fd) == -1)
// 					(perror("close"), exit(EXIT_FAILURE)); // free also
// 			}
// 			if (temp->smplcmd->builtin == 0)
// 				exec_simple_command(temp->smplcmd, shell);
// 			else
// 				exec_builtin(temp->smplcmd, shell);
// 		}
// 		else
// 		{
// 			if (prev_fd != STDIN_FILENO)
// 				close(prev_fd);
// 			if (temp->next)
// 			{
// 				close(fd[1]);
// 				prev_fd = fd[0];
// 			}
// 			else
// 				prev_fd = STDIN_FILENO;
// 			waitpid(pid, NULL, 0);
// 		}
// 		temp = temp->next;
// 	}
// 	free_structs(&cmds);
// 	return ;
// }

int	handle_redirect(char *filename, int open_flags, int redirect_target)
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

int	handle_redirections(t_redirection *redirections)
{
	while (redirections)
	{
		if (redirections->type == 1)
		{
			if (handle_redirect(redirections->file, O_RDONLY, STDIN_FILENO) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (redirections->type == 2)
		{
			exec_heredoc(redirections, 1);
			if (handle_redirect(redirections->file, O_RDONLY, STDIN_FILENO) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (redirections->type == 3)
		{
			if (handle_redirect(redirections->file, O_TRUNC | O_CREAT | O_RDWR, STDOUT_FILENO) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else
		{
			if (handle_redirect(redirections->file, O_APPEND | O_CREAT | O_RDWR, STDOUT_FILENO) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		redirections = redirections->next;
	}
	return (EXIT_SUCCESS);
}

int	init_pipe(t_cmd *cmd)
{
	if (cmd->next)
	{
		if (pipe(cmd->pipe) == -1)
			(perror("pipe"), exit(EXIT_FAILURE)); //free also
	}
	return (EXIT_SUCCESS);
}

void	handle_child_process(t_cmd *cmd, t_shell *shell)
{
	if (cmd->prev)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1)
			return (perror("dup2"), exit(EXIT_FAILURE)); // free also
		if (close(cmd->prev->pipe[0]) == -1)
			return (perror("close"), exit(EXIT_FAILURE)); // free and close others if hasNext also
	}
	if (cmd->next)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
			return (perror("dup2"), exit(EXIT_FAILURE)); // free also
		// if (close(cmd->pipe[0]) == -1)
		// 	return (perror("close"), close(cmd->pipe[1]), exit(EXIT_FAILURE)); // free also
		if (close(cmd->pipe[1]) == -1)
			return (perror("close"), exit(EXIT_FAILURE)); // free also
		if (close(cmd->pipe[0]) == -1)
			return (perror("close"), exit(EXIT_FAILURE)); // free also
	}		
	if (handle_redirections(cmd->smplcmd->redir))
		exit(EXIT_FAILURE); //TODO: Error Handling
	//TODO: Refactor this
	if (cmd->smplcmd->builtin == 0)
		exit(exec_simple_command(cmd->smplcmd, shell));
	else
		exit(exec_builtin(cmd->smplcmd, shell));
}

int	handle_parent_process(t_cmd *cmd)
{
	if (cmd->prev)
	{
		if (close(cmd->prev->pipe[0]) == -1)
			return (perror("close"), EXIT_FAILURE); // free also
	}
	if (cmd->next)
	{
		if (close(cmd->pipe[1]) == -1)
			return (perror("close"), EXIT_FAILURE); // free and close also
	}
	return (EXIT_SUCCESS);
}

int	handle_single_command(t_shell *shell, t_cmd *cmd)
{
	if (handle_redirections(cmd->smplcmd->redir))
		return (EXIT_FAILURE); //TODO: Error Handling
	//TODO: Refactor this
	if (cmd->smplcmd->builtin == 0)
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
			exit(exec_simple_command(cmd->smplcmd, shell));
	}
	else
		shell->last_exit_code = exec_builtin(cmd->smplcmd, shell);
	return (EXIT_SUCCESS);
}

int	handle_multiple_commands(t_shell *shell, t_cmd *cmd)
{
	while (cmd)
	{
		if (init_pipe(cmd) != EXIT_SUCCESS)
			return (EXIT_FAILURE); //TODO: Error Handling
		cmd->pid = fork();
		if (cmd->pid == -1)
			(perror("fork"), exit(EXIT_FAILURE)); // free all cmds before
		else if (cmd->pid == 0)
			handle_child_process(cmd, shell);
		else
		{
			if (handle_parent_process(cmd) != EXIT_SUCCESS)
				return (EXIT_FAILURE); //TODO: Error Handling;
		}
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

int	handle_waiting_processes(t_cmd *cmd, t_shell *shell)
{
	int	status;

	status = 0;
	while (cmd)
	{
		if (waitpid(cmd->pid, &status, 0) == -1)
		{
			perror("waitpid"); //TODO: Free and close stuff
			printf("PID: %d\n", cmd->pid);
			return (EXIT_FAILURE); //TODO: Error Handling
		}
		status = WEXITSTATUS(status);
		//TODO: Heredoc check with deleting tmp file
		cmd = cmd->next;
	}
	shell->last_exit_code = status;
	return (EXIT_SUCCESS);
}

void	exec_commands(char *cmd, t_shell *shell)
{
	t_cmd	*cmds;

	cmds = parse_commands(cmd, shell);
	if (!cmds)
		return ;
	if (cmds->next)
	{
		if (handle_multiple_commands(shell, cmds) != EXIT_SUCCESS)
			return ; //TODO: Error Handling
	}
	else
	{
		if (handle_single_command(shell, cmds) != EXIT_SUCCESS)
			return ; //TODO: Error Handling
	}
	if (cmds->next || cmds->smplcmd->builtin == 0)
	{
		if (handle_waiting_processes(cmds, shell) != EXIT_SUCCESS)
			return ; //TODO: Error Handling
	}
	free_structs(&cmds);
	return ;
}


// also need to handle ctrl + c and ctrl + d here
// int	redirect_input_output(t_redirection *redir)
// {
// 	int fd;

// 	if (redir->type == 1)
// 	{
// 		fd = open(redir->file, O_RDONLY);
// 		if (fd == -1)
// 			return (perror(redir->file), 1);
// 		if (dup2(fd, STDIN_FILENO) == -1)
// 			return (perror("dup2"), close(fd), 1);
// 		if (close(fd) == -1)
// 			return (perror("close")), 1;
// 	}
// 	else if (redir->type == 2)
// 	{
// 		return (exec_heredoc(redir));
// 	}
// 	else if (redir->type == 3)
// 	{
// 		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd == -1)
// 			return (perror(redir->file), 1);
// 		if (dup2(fd, STDOUT_FILENO) == -1)
// 			return (perror("dup2"), close(fd), 1);
// 		if (close(fd) == -1)
// 			return (perror("close"), 1);
// 	}
// 	else if (redir->type == 4)
// 	{
// 		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd == -1)
// 			return (perror(redir->file), 1);
// 		if (dup2(fd, STDOUT_FILENO) == -1)
// 			return (perror("dup2"), close(fd), 1);
// 		if (close(fd) == -1)
// 			return (perror("close"), 1);
// 	}
// 	return (1);
// }

int	exec_simple_command(t_smplcmd *smplcmd, t_shell *shell)
{
	char	*path;
	char	**args;
	//char	**env_path;

	(void)shell; // temp
	args = smplcmd->args;
	path = smplcmd->path;
	// env_path = get_env_path(shell->env);
	// if (!env_path)
	// 	env_path = get_env_path(shell->exported_vars);
	// path = get_path(env_path, smplcmd->path);
	// need to handle exit code here
	if (path && execve(path, args, NULL) == -1) // need to set env 
		return (free(path), perror("execve"), 1);
	else
		return (free(path), 1);
}
