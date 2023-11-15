/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:26:38 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/15 12:29:32 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc_execution(t_cmd *cmds, t_shell *shell)
{
	int		i;
	t_cmd	*temp;
	t_redirection *redir;

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
