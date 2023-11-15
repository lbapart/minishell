/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:26:38 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/15 15:35:50 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc_execution(t_cmd *cmds, t_shell *shell)
{
	int				i;
	t_cmd			*temp;
	t_redirection	*redir;

	i = 0;
	temp = cmds;
	init_signals(HEREDOC_MODE);
	while (temp)
	{
		redir = temp->smplcmd->redir;
		while (redir != NULL)
		{
			if (redir->type == 2)
			{
				if (exec_heredoc(redir, i, shell) != EXIT_SUCCESS)
					return (EXIT_FAILURE);
			}
			i++;
			redir = redir->next;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

int	delete_tmp_heredocs_files(t_cmd	*cmd)
{
	t_redirection	*redir;
	int				exit_code;

	exit_code = EXIT_SUCCESS;
	while (cmd)
	{
		redir = cmd->smplcmd->redir;
		while (redir != NULL)
		{
			if (redir->type == 2 && redir->to_delete == 1)
			{
				if (unlink(redir->file) != 0)
				{
					perror("unlink");
					exit_code = EXIT_FAILURE;
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (exit_code);
}
