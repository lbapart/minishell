/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:32:40 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 23:08:03 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(smplcmd->path);
	free(smplcmd);
}

void	free_structs(t_cmd **cmds)
{
	t_cmd			*tmp;
	t_smplcmd		*smplcmd;

	tmp = *cmds;
	while (tmp)
	{
		smplcmd = tmp->smplcmd;
		free_smplcmd(smplcmd);
		*cmds = tmp;
		tmp = tmp->next;
		free(*cmds);
		*cmds = NULL;
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

void	free_everything(char **tokens, t_cmd *cmd, t_smplcmd *smplcmd)
{
	free_dbl_ptr(tokens);
	free_smplcmd(smplcmd);
	free_structs(&cmd);
}
