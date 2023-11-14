/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:21:12 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:25:16 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	do_redirection(char *filename, int open_flags, int redirect_target)
{
	int	fd;

	fd = open(filename, open_flags, 0000644);
	if (fd == -1)
		return (perror(filename), EXIT_FAILURE);
	if (dup2(fd, redirect_target) == -1)
		return (perror("dup2"), close(fd), EXIT_FAILURE);
	if (close(fd) == -1)
		return (perror("close"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_redirection_per_type(t_redirection *redirection)
{
	if (redirection->type == 1 || redirection->type == 2)
		return (do_redirection(redirection->file,
				O_RDONLY, STDIN_FILENO));
	else if (redirection->type == 3)
		return (do_redirection(redirection->file,
				O_TRUNC | O_CREAT | O_RDWR, STDOUT_FILENO));
	else
		return (do_redirection(redirection->file,
				O_APPEND | O_CREAT | O_RDWR, STDOUT_FILENO));
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
