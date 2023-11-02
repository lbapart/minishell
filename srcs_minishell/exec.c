/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:45:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/02 17:30:56 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	exec_builtin(t_smplcmd *smplcmd)
{
	printf("builtin\n");
}

int	exec_heredoc(t_redirection *redir)
{
	printf("heredoc\n");
	return (1);
}

void	exec_commands(char *cmd)
{
	t_cmd	*cmds;
	t_cmd	*temp;
	int		fd[2];
	int		prev_fd = STDIN_FILENO;

	cmds = parse_commands(cmd);
	if (!cmds)
		return ;
	temp = cmds;
	while (temp)
	{
		if (temp->next)
		{
			if (pipe(fd) == -1)
				(perror("pipe"), exit(EXIT_FAILURE)); //free also
		}
		pid_t pid = fork();
		if (pid == -1)
			(perror("fork"), exit(EXIT_FAILURE)); // free also	
		else if (pid == 0)
		{
			if (temp->next)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					(perror("dup2"), exit(EXIT_FAILURE)); // free also
				if (close(fd[0]) == -1 || close(fd[1]) == -1)
					(perror("close"), exit(EXIT_FAILURE)); // free also
			}
			if (prev_fd != STDIN_FILENO)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					(perror("dup2"), exit(EXIT_FAILURE)); // free also
				if (close(prev_fd) == -1)
					(perror("close"), exit(EXIT_FAILURE)); // free also
			}
			if (temp->smplcmd->builtin == 0)
				exec_simple_command(temp->smplcmd);
			else
				exec_builtin(temp->smplcmd);
		}
		else
		{
			if (prev_fd != STDIN_FILENO)
				close(prev_fd);
			if (temp->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
			else
				prev_fd = STDIN_FILENO;
			waitpid(pid, NULL, 0);
		}
		temp = temp->next;
	}
	free_structs(cmds);
	return ;
}

// also need to handle ctrl + c and ctrl + d here
int	redirect_input_output(t_redirection *redir)
{
	int fd;

	if (redir->type == 1)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
			return (perror(redir->file), 1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(fd), 1);
		if (close(fd) == -1)
			return (perror("close")), 1;
	}
	else if (redir->type == 2)
	{
		return (exec_heredoc(redir));
	}
	else if (redir->type == 3)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror(redir->file), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), close(fd), 1);
		if (close(fd) == -1)
			return (perror("close"), 1);
	}
	else if (redir->type == 4)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (perror(redir->file), 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), close(fd), 1);
		if (close(fd) == -1)
			return (perror("close"), 1);
	}
	return (1);
}

int	exec_simple_command(t_smplcmd *smplcmd)
{
	t_redirection	*temp;
	char	*path;
	char	**args;

	temp = smplcmd->redir;
	path = NULL;
	args = smplcmd->args;
	while (temp)
	{
		redirect_input_output(temp);
		temp = temp->next;
	}
	if (!smplcmd->path && smplcmd->args)
	{
		path = smplcmd->args[0];
		args = &smplcmd->args[1];
	}
	else if (smplcmd->path)
	{
		path = smplcmd->path;
		args = smplcmd->args;
	}
	// need to handle exit code here
	if (path && execve(path, args, NULL) == -1)
			return (perror("execve"), 1);
	else
		return (1);
}
