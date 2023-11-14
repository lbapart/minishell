/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_multiple_commands.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:03:10 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 15:03:18 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
