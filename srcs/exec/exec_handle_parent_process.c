/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_parent_process.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:56:12 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:58:48 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (perror("close"), close(cmd->pipe[0]), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
