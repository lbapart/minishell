/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:32:40 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 21:00:36 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_smplcmd(t_smplcmd *smplcmd)
{
	size_t			i;
	t_redirection	*temp;
	t_redirection	*temp2;

	i = 0;
	while (smplcmd->args && smplcmd->args[i])
		free(smplcmd->args[i++]);
	free(smplcmd->args);
	temp = smplcmd->redir;
	while (temp)
	{
		free(temp->file);
		temp2 = temp;
		temp = temp->next;
		free(temp2);
	}
	free(smplcmd->redir);
	free(smplcmd->path);
	free(smplcmd->cmd);
	free(smplcmd);
}

void	free_structs(t_cmd *cmds)
{
	t_cmd			*tmp;
	t_smplcmd		*smplcmd;
	t_redirection	*temp;
	size_t			i;

	tmp = cmds;
	while (tmp)
	{
		free(tmp->cmd);
		smplcmd = tmp->smplcmd;
		free_smplcmd(smplcmd);
		cmds = tmp;
		tmp = tmp->next;
		free(cmds);
	}
}

void	free_dbl_ptr(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr && ptr[i])
		free(ptr[i++]);
	free(ptr);
}

void	free_and_null(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}
