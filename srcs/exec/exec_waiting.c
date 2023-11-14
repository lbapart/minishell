/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_waiting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:27:55 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 15:16:26 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_all_commands_on_error(t_cmd *start, t_cmd *cmd)
{
	int		status;

	status = EXIT_FAILURE;
	while (start != cmd)
	{
		waitpid(start->pid, &status, 0);
		start = start->next;
	}
	return (EXIT_FAILURE);
}

static void	delete_tmp_heredocs_files(t_cmd	*cmd, int *is_error)
{
	while (cmd)
	{
		if (cmd->smplcmd->redir != NULL && cmd->smplcmd->redir->type == 2
			&& cmd->smplcmd->redir->to_delete == 1)
		{
			if (unlink(cmd->smplcmd->redir->file) != 0)
			{
				perror("unlink");
				*is_error = 1;
			}
		}
		cmd = cmd->next;
	}
}

static int	wait_get_last_exit_code(t_cmd *cmd, int *is_error)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	while (cmd)
	{
		if (waitpid(cmd->pid, &exit_code, 0) == -1)
		{
			perror("waitpid");
			*is_error = 1;
		}
		exit_code = WEXITSTATUS(exit_code);
		cmd = cmd->next;
	}
	return (exit_code);
}

int	handle_waiting_processes(t_cmd *cmd, t_shell *shell)
{
	int		exit_code;
	int		is_error;

	is_error = 0;
	exit_code = wait_get_last_exit_code(cmd, &is_error);
	delete_tmp_heredocs_files(cmd, &is_error);
	if (is_error)
		return (EXIT_FAILURE);
	shell->last_exit_code = exit_code;
	return (EXIT_SUCCESS);
}
