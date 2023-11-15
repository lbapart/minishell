/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:45:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/15 13:14:45 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_up(t_cmd *cmds, t_shell *shell)
{
	if (delete_tmp_heredocs_files(cmds) != EXIT_SUCCESS)
		shell->last_exit_code = EXIT_FAILURE;
	free_structs(&cmds);
}

void	exec_commands(char *cmd, t_shell *shell)
{
	t_cmd	*cmds;

	cmds = parse_commands(cmd, shell);
	if (!cmds)
		return ;
	if (init_heredoc_execution(cmds, shell) != EXIT_SUCCESS)
		return (free_set_failure_unlink(&cmds, shell));
	init_signals(GLOBAL_MODE);
	if (cmds->next)
	{
		if (handle_multiple_commands(shell, cmds) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	else
	{
		if (handle_single_command(shell, cmds) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	if (cmds->next || cmds->smplcmd->builtin == 0)
	{
		if (handle_waiting_processes(cmds, shell) != EXIT_SUCCESS)
			return (free_set_failure_unlink(&cmds, shell));
	}
	return (clean_up(cmds, shell));
}