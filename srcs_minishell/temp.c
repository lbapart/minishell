/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:43:18 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 20:44:25 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//functions for testing
#include "parsing.h"

void	print_commands(t_cmd *cmds)
{
	t_cmd *tmp;
	t_smplcmd *smplcmd;
	t_redirection *temp;
	size_t i;
	size_t count = 1;

	tmp = cmds;
	while (tmp)
	{
		printf("--------------------\n");
		printf("cmd: %s\n", tmp->cmd);
		smplcmd = tmp->smplcmd;
		printf("path: %s\n", smplcmd->path);
		i = 0;
		while (smplcmd->args && smplcmd->args[i])
		{
			printf("arg %zu: %s\n", i, smplcmd->args[i]);
			i++;
		}
		temp = smplcmd->redir;
		while (temp)
		{
			printf("redir type: %d\n", temp->type);
			printf("redir file: %s\n", temp->file);
			temp = temp->next;
		}
		tmp = tmp->next;
		printf("--------------------\n");
	}
}