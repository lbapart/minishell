/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:23:53 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:09:27 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_exit(t_pipex *pipex, t_command *command, int exit_status)
{
	if (pipex)
		free_pipex(pipex);
	if (command)
		free_command(command);
	exit(exit_status);
}

static void	close_pipe_end(int pipe, t_pipex *pex, t_command *cmd)
{
	if (close(pipe) == -1)
	{
		perror("Pipe closing error");
		free_exit(pex, cmd, EXIT_FAILURE);
	}
}

static void	redirect_input_output(int out, int in, t_pipex *p, t_command *cmd)
{
	if (dup2(out, STDOUT_FILENO) == -1)
		handle_redirect_error(out, in, p, cmd);
	if (dup2(in, STDIN_FILENO) == -1)
		handle_redirect_error(out, in, p, cmd);
}

void	execute_command(t_pipex *pex, t_execute_command_data d)
{
	if (close(d.unused_pipe) == -1)
	{
		perror("Pipe closing error");
		close(d.file_fd);
		close(d.used_pipe);
		free_exit(pex, d.command, EXIT_FAILURE);
	}
	redirect_input_output(d.output_redirect, d.input_redirect, pex, d.command);
	if (close(d.file_fd) == -1)
	{
		perror(d.filename);
		close(d.used_pipe);
		free_exit(pex, d.command, EXIT_FAILURE);
	}
	close_pipe_end(d.used_pipe, pex, d.command);
	execve(d.command->path, d.command->args, NULL);
	perror("execve error");
	free_exit(pex, d.command, EXIT_FAILURE);
}
