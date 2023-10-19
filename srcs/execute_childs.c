/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_childs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:16:31 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:46:31 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	handle_fork_error(t_pipex *pipex)
{
	perror("Error while forking");
	close(pipex->outfile_fd);
	close_pipe(pipex->pipefd);
	free_pipex(pipex);
	exit(EXIT_FAILURE);
}

static int	open_else_clean_exit(char *in, t_pipex *pipex)
{
	int	input_fd;

	input_fd = open(in, O_RDONLY);
	if (input_fd == -1)
	{
		close_pipe(pipex->pipefd);
		free_pipex(pipex);
		exit(EXIT_FAILURE);
	}
	return (input_fd);
}

void	execute_first_child(t_pipex *pipex)
{
	t_execute_command_data	data;

	pipex->pid_child_1 = fork();
	if (pipex->pid_child_1 == -1)
		handle_fork_error(pipex);
	if (pipex->pid_child_1 != 0)
		return ;
	if (close(pipex->outfile_fd))
	{
		perror(pipex->argv[4]);
		close_pipe(pipex->pipefd);
		free_pipex(pipex);
		exit(EXIT_FAILURE);
		return ;
	}
	data.filename = pipex->argv[1];
	data.file_fd = open_else_clean_exit(data.filename, pipex);
	data.command = parse_command(pipex->argv[2], pipex->paths);
	if (!data.command)
		handle_command_error(pipex, pipex->argv[2], data.file_fd);
	data.unused_pipe = pipex->pipefd[0];
	data.used_pipe = pipex->pipefd[1];
	data.output_redirect = pipex->pipefd[1];
	data.input_redirect = data.file_fd;
	execute_command(pipex, data);
}

void	execute_second_child(t_pipex *pipex)
{
	t_execute_command_data	data;

	pipex->pid_child_2 = fork();
	if (pipex->pid_child_2 == -1)
		handle_fork_error(pipex);
	if (pipex->pid_child_2 != 0)
		return ;
	data.command = parse_command(pipex->argv[3], pipex->paths);
	if (!data.command)
		handle_command_error(pipex, pipex->argv[3], pipex->outfile_fd);
	data.filename = pipex->argv[4];
	data.file_fd = pipex->outfile_fd;
	data.unused_pipe = pipex->pipefd[1];
	data.used_pipe = pipex->pipefd[0];
	data.output_redirect = pipex->outfile_fd;
	data.input_redirect = pipex->pipefd[0];
	execute_command(pipex, data);
}
