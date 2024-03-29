/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:52:41 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 16:31:22 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_smplcmd	*init_simple_command(void)
{
	t_smplcmd	*smplcmd;

	smplcmd = (t_smplcmd *)malloc(sizeof(t_smplcmd));
	if (!smplcmd)
		return (NULL);
	smplcmd->path = NULL;
	smplcmd->args = NULL;
	smplcmd->builtin = 0;
	smplcmd->redir = NULL;
	return (smplcmd);
}

t_redirection	*init_redir(void)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->file = NULL;
	redir->type = 0;
	redir->next = NULL;
	redir->to_delete = 0;
	return (redir);
}

t_cmd	*init_new_cmd(t_shell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->smplcmd = NULL;
	new_cmd->shell = shell;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	return (new_cmd);
}

void	init_vars(t_pars_vars *vars, char *cmd, t_shell *shell)
{
	vars->i = 0;
	vars->j = 0;
	vars->last_pipe = 0;
	vars->temp = NULL;
	vars->cmds = NULL;
	vars->is_open_single_quote = 0;
	vars->is_open_double_quote = 0;
	vars->smplcmd = NULL;
	vars->tokens = NULL;
	vars->new_cmd = NULL;
	vars->cmd_to_exec = NULL;
	vars->res = NULL;
	vars->tc = 0;
	vars->in_quotes = NOQUOTES;
	vars->start = cmd;
	vars->end = cmd;
	vars->hr = 1;
	vars->redir = NULL;
	vars->shell = shell;
	vars->iosq = 0;
	vars->iodq = 0;
}
