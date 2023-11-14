/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:35:32 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 15:04:49 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_child_process(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*begin;

	begin = cmd;
	while (begin->prev)
		begin = begin->prev;
	free_structs(&begin);
	free_all_envs(&(shell->exported_vars));
	free_all_envs(&(shell->env));
	rl_clear_history();
}

void	free_set_failure_unlink(t_cmd **cmds, t_shell *shell)
{
	t_cmd	*temp;

	temp = *cmds;
	while (temp)
	{
		if (temp->smplcmd->redir != NULL && temp->smplcmd->redir->type == 2
			&& temp->smplcmd->redir->to_delete == 1)
		{
			unlink(temp->smplcmd->redir->file);
		}
		temp = temp->next;
	}
	free_structs(cmds);
	shell->last_exit_code = EXIT_FAILURE;
	return ;
}

int	handle_fork_error(t_cmd *start, t_cmd *cmd)
{
	perror("fork");
	close(cmd->pipe[0]);
	close(cmd->pipe[1]);
	return (handle_pipe_error(start, cmd));
}

int	handle_pipe_error(t_cmd *start, t_cmd *cmd)
{
	if (cmd->prev)
		close(cmd->prev->pipe[0]);
	return (wait_all_commands_on_error(start, cmd));
}
