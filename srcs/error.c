/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 11:32:45 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:25:54 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (pipex->paths)
	{
		while (pipex->paths[i])
		{
			free(pipex->paths[i]);
			i++;
		}
		free(pipex->paths);
	}
	free(pipex);
}

void	close_pipe(int pipe[])
{
	close(pipe[0]);
	close(pipe[1]);
}

void	handle_command_error(t_pipex *pipex, char *command_name, int fd)
{
	ft_putstr_fd(command_name, 2);
	ft_putendl_fd(": command not found", 2);
	close_pipe(pipex->pipefd);
	close(fd);
	free_pipex(pipex);
	exit(127);
}

void	handle_redirect_error(int file, int pipe, t_pipex *p, t_command *c)
{
	perror("dup2 Error");
	close(pipe);
	close(file);
	free_pipex(p);
	free_command(c);
	exit(EXIT_FAILURE);
}
