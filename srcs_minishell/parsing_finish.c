/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_finish.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:22:49 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/31 01:40:33 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	set_builtin(t_cmd *cmd)
{
	t_cmd	*temp;
	t_smplcmd	*smplcmd;

	temp = cmd;
	while (temp)
	{
		smplcmd = temp->smplcmd;
		if (ft_strcmp(smplcmd->args[0], "echo") == 0)
			smplcmd->builtin = ECHO;
		else if (ft_strcmp(smplcmd->args[0], "cd") == 0)
			smplcmd->builtin = CD;
		else if (ft_strcmp(smplcmd->args[0], "pwd") == 0)
			smplcmd->builtin = PWD;
		else if (ft_strcmp(smplcmd->args[0], "export") == 0)
			smplcmd->builtin = EXPORT;
		else if (ft_strcmp(smplcmd->args[0], "unset") == 0)
			smplcmd->builtin = UNSET;
		else if (ft_strcmp(smplcmd->args[0], "env") == 0)
			smplcmd->builtin = ENV;
		else if (ft_strcmp(smplcmd->args[0], "exit") == 0)
			smplcmd->builtin = EXIT;
		temp = temp->next;
	}
}

t_cmd	*finish_pars(t_cmd *cmd)
{
	t_cmd	*temp;
	size_t	count;
	
	temp = cmd;
	count = 0;
	while (temp)
	{
		if (!temp->smplcmd->args)
		{
			if (temp->next || count > 0)
				return (free_structs(cmd), unexpected_near_pipe_err(), NULL);
			else
				return (NULL);
		}
		count++;
	}
	set_builtin(cmd);
	return (cmd);
}