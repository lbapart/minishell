/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:26:38 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:27:00 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc_execution(t_cmd *cmds, t_shell *shell)
{
	int		i;
	t_cmd	*temp;

	i = 0;
	temp = cmds;
	init_signals(HEREDOC_MODE);
	while (temp)
	{
		if (temp->smplcmd->redir != NULL && temp->smplcmd->redir->type == 2)
		{
			if (exec_heredoc(temp->smplcmd->redir, i, shell) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
			i++;
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}
