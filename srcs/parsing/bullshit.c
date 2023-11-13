/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bullshit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:14:33 by aapenko           #+#    #+#             */
/*   Updated: 2023/11/13 18:17:21 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_bullshit_in_str(char *str)
{
	size_t	i;

	i = 0;
	while (str[i + 1])
	{
		str[i] = str[i + 1];
		i++;
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == BULLSHIT)
			str[i] = '\0';
		i++;
	}
}

void	replace_bullshit(t_cmd *cmd)
{
	t_cmd			*temp;
	t_redirection	*redir;
	size_t			i;

	temp = cmd;
	while (temp)
	{
		if (temp->smplcmd->path && temp->smplcmd->path[0] == BULLSHIT)
			replace_bullshit_in_str(temp->smplcmd->path);
		i = 0;
		while (temp->smplcmd->args && temp->smplcmd->args[i])
		{
			if (temp->smplcmd->args[i][0] == BULLSHIT)
				replace_bullshit_in_str(temp->smplcmd->args[i]);
			i++;
		}
		redir = temp->smplcmd->redir;
		while (redir)
		{
			if (redir->file && redir->file[0] == BULLSHIT)
				replace_bullshit_in_str(redir->file);
			redir = redir->next;
		}
		temp = temp->next;
	}
}
