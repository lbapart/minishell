/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:45:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/08 08:50:42 by lbapart          ###   ########.fr       */
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
		return (execute_exit(*smplcmd));
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

void	exec_commands(char *cmd, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*temp;

	cmds = parse_commands(cmd, shell);
	if (!cmds)
		return ;
	temp = cmds;
	if (!temp->next)
	{
		t_redirection *redirections = temp->smplcmd->redir;
			int	file_descriptor;
			while (redirections)
			{
				// 0 for no redirection, 1 for <, 2 for <<, 3 for >, 4 for >>
				printf("%s: type=%d next=%p\n", redirections->file, redirections->type, redirections->next);
				if (redirections->type == 1)
				{
					//TODO: Input redirect
					file_descriptor = open(redirections->file, O_RDONLY, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDIN_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				else if (redirections->type == 2)
				{
					//TODO: Heredoc: Call heredoc function and then redirect STDIN to filedescriptor
					exec_heredoc(redirections, 1);
					file_descriptor = open(redirections->file, O_RDONLY, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDIN_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE));
				}
				else if (redirections->type == 3)
				{
					//TODO: Output redirect
					file_descriptor = open(redirections->file, O_TRUNC | O_CREAT | O_RDWR, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDOUT_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				else
				{
					//TODO: Output append redirect
					file_descriptor = open(redirections->file, O_APPEND | O_CREAT | O_RDWR, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDOUT_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				redirections = redirections->next;
			}
			//TODO: Refactor this
			if (temp->smplcmd->builtin == 0)
			{
				temp->pid = fork();
				if (temp->pid == 0)
				{
					exec_simple_command(temp->smplcmd, shell);
					return ;
				}
			}
			else
				exec_builtin(temp->smplcmd, shell);
	}
	else
	{
	while (temp)
	{
		if (temp->next)
		{
			if (pipe(temp->pipe) == -1)
				(perror("pipe"), exit(EXIT_FAILURE)); //free also
		}
		temp->pid = fork();
		if (temp->pid == -1)
			(perror("fork"), exit(EXIT_FAILURE)); // free all cmds before
		else if (temp->pid == 0) //Child Process
		{
			if (temp->prev)
			{
				if (dup2(temp->prev->pipe[0], STDIN_FILENO) == -1)
					return (perror("dup2"), exit(EXIT_FAILURE)); // free also
				if (close(temp->prev->pipe[0]) == -1)
					return (perror("close"), exit(EXIT_FAILURE)); // free and close others if hasNext also
			}
			if (temp->next)
			{
				if (dup2(temp->pipe[1], STDOUT_FILENO) == -1)
					return (perror("dup2"), exit(EXIT_FAILURE)); // free also
				// if (close(temp->pipe[0]) == -1)
				// 	return (perror("close"), close(temp->pipe[1]), exit(EXIT_FAILURE)); // free also
				if (close(temp->pipe[1]) == -1)
					return (perror("close"), exit(EXIT_FAILURE)); // free also
				
				if (close(temp->pipe[0]) == -1)
					return (perror("close"), exit(EXIT_FAILURE)); // free also
				
			}
			t_redirection *redirections = temp->smplcmd->redir;
			int	file_descriptor;
			while (redirections)
			{
				// 0 for no redirection, 1 for <, 2 for <<, 3 for >, 4 for >>
				printf("%s: type=%d next=%p\n", redirections->file, redirections->type, redirections->next);
				if (redirections->type == 1)
				{
					//TODO: Input redirect
					file_descriptor = open(redirections->file, O_RDONLY, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDIN_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				else if (redirections->type == 2)
				{
					//TODO: Heredoc: Call heredoc function and then redirect STDIN to filedescriptor
					exec_heredoc(redirections, temp->pid);
					file_descriptor = open(redirections->file, O_RDONLY, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDIN_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE));
				}
				else if (redirections->type == 3)
				{
					//TODO: Output redirect
					file_descriptor = open(redirections->file, O_TRUNC | O_CREAT | O_RDWR, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDOUT_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				else
				{
					//TODO: Output append redirect
					file_descriptor = open(redirections->file, O_APPEND | O_CREAT | O_RDWR, 0000644);
					if (file_descriptor == -1)
						return (perror(redirections->file));
					if (dup2(file_descriptor, STDOUT_FILENO) == -1)
						return (perror("dup2"), close(file_descriptor), exit(EXIT_FAILURE)); // free also
					if (close(file_descriptor) == -1)
						return (perror("close"), exit(EXIT_FAILURE)); // free also
				}
				redirections = redirections->next;
			}
			//TODO: Refactor this
			if (temp->smplcmd->builtin == 0)
				exec_simple_command(temp->smplcmd, shell);
			else
				exec_builtin(temp->smplcmd, shell);
			return ;
		}
		else //parent process
		{
			if (temp->prev)
			{
				if (close(temp->prev->pipe[0]) == -1)
					return (perror("close"), exit(EXIT_FAILURE)); // free also
			}
			if (temp->next)
			{
				if (close(temp->pipe[1]) == -1)
					return (perror("close"), exit(EXIT_FAILURE)); // free and close also
				
			}
		}
		temp = temp->next;
	}
	}
	temp = cmds;
	int status = 0;
	if (temp->next || temp->smplcmd->builtin == 0)
	{
		while (temp)
		{
			if (waitpid(temp->pid, &status, 0) == -1)
			{
				perror("waitpid"); //TODO: Free and close stuff
				printf("PID: %d\n", temp->pid);
			}
			status = WEXITSTATUS(status);
			//TODO: Heredoc check with deleting tmp file
			temp = temp->next;
		}
	}
	
	shell->last_exit_code = status;
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
	char	**env_path;

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
