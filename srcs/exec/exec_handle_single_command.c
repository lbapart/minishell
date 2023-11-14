/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_single_command.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:59:02 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 16:59:25 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_command(t_shell *shell, t_cmd *cmd)
{
	int	exit_redirections;

	exit_redirections = handle_redirections(cmd->smplcmd->redir);
	if (exit_redirections != EXIT_SUCCESS)
		return (exit_redirections);
	if (cmd->smplcmd->builtin == 0)
	{
		init_signals(MAIN_WHEN_CHILD_MODE);
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (perror("fork"), EXIT_FAILURE);
		if (cmd->pid == 0)
		{
			close_unused_fds(cmd, shell);
			exit(exec_simple_command(cmd->smplcmd, shell));
		}
	}
	else
		shell->last_exit_code = exec_builtin(cmd->smplcmd, shell);
	return (EXIT_SUCCESS);
}
