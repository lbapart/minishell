/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:52:41 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 21:01:01 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_smplcmd	*init_simple_command(t_cmd *cmd)
{
	t_smplcmd *smplcmd;

	smplcmd = (t_smplcmd *)malloc(sizeof(t_smplcmd));
	if (!smplcmd)
		return (NULL); // throw error here and free everything and exit
	smplcmd->cmd = NULL;
	smplcmd->path = NULL;
	smplcmd->args = NULL;
	smplcmd->bultin = NULL;
	smplcmd->redir = NULL;
	return (smplcmd);
}

t_redirection *init_redir(void)
{
	t_redirection *redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL); // throw error here and free everything and exit
	redir->file = NULL;
	redir->type = 0;
	redir->next = NULL;
	return (redir);
}

t_cmd	*init_new_cmd(char *cmd, t_cmd *cmds)
{
	t_cmd *new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL); // throw error here and free everything and exit
	new_cmd->cmd = cmd;
	if (!new_cmd->cmd)
		return (NULL); // throw error here and free everything and exit
	new_cmd->smplcmd = NULL;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->signal_received = 0;
	return (new_cmd);
}