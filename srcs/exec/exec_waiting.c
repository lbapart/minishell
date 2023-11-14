/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_waiting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:27:55 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 20:10:25 by aapenko          ###   ########.fr       */
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
	t_redirection	*redir;
	while (cmd)
	{
		redir = cmd->smplcmd->redir;
		while (redir != NULL && redir->type == 2
			&& redir->to_delete == 1)
		{
			if (unlink(redir->file) != 0)
			{
				perror("unlink");
				*is_error = 1;
			}
			redir = redir->next;
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
