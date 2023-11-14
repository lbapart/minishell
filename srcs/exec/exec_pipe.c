/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:29:11 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 15:05:49 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_pipe(t_cmd *cmd)
{
	if (cmd->next)
	{
		if (pipe(cmd->pipe) == -1)
			return (perror("pipe"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	redirect_pipe_input(t_cmd *cmd, t_shell *shell)
{
	if (cmd->prev)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close_pipe_free_process_exit(cmd, shell));
		if (close(cmd->prev->pipe[0]) == -1)
		{
			perror("close");
			if (cmd->next)
				close(cmd->pipe[1]);
			free_child_process(cmd, shell);
			return (exit(EXIT_FAILURE));
		}
	}
}

void	redirect_pipe_output(t_cmd *cmd, t_shell *shell)
{
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
			return (perror("close"), free_child_process(cmd, shell),
				exit(EXIT_FAILURE));
	}
}

void	close_pipe(t_cmd *cmd)
{
	if (cmd->prev)
		close(cmd->prev->pipe[0]);
	if (cmd->next)
		close(cmd->pipe[1]);
}
