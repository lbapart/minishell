/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_finish.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 22:22:49 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/08 12:08:53 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_builtin(t_cmd *cmd)
{
	t_cmd		*temp;
	t_smplcmd	*smplcmd;

	temp = cmd;
	while (temp && temp->smplcmd->args)
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

void	unset_hidden_quotes(char *arg)
{
	size_t	i;

	i = 0;
	while (arg && arg[i])
	{
		if (arg[i] == HIDDEN_QUOTE)
			arg[i] = '\'';
		i++;
	}
}

void	unset_hidden_dollar(t_cmd *cmd)
{
	t_cmd			*temp;
	t_redirection	*redir;
	size_t			i;

	temp = cmd;
	while (temp)
	{
		redir = temp->smplcmd->redir;
		while (redir)
		{
			i = 0;
			while (redir->file && redir->file[i])
			{
				if (redir->file[i] == HEREDOC_HIDDEN_DOLLAR)
					redir->file[i] = '$';
				i++;
			}
			redir = redir->next;
		}
		temp = temp->next;
	}
}

t_cmd	*finish_pars(t_cmd *cmd)
{
	t_cmd	*temp;
	size_t	count;
	size_t	i;

	temp = cmd;
	count = 0;
	while (temp)
	{
		i = 0;
		if (temp->smplcmd->path)
			unset_hidden_quotes(temp->smplcmd->path);
		while (temp->smplcmd->args && temp->smplcmd->args[i])
			unset_hidden_quotes(temp->smplcmd->args[i++]);
		if (!temp->smplcmd->args && !temp->smplcmd->redir)
		{
			if (temp->next || count > 0)
				return (free_structs(&cmd), unexpected_near_pipe_err(), NULL);
			else
				return (free_structs(&cmd), NULL);
		}
		count++;
		temp = temp->next;
	}
	(unset_hidden_dollar(cmd), set_prev_cmds(cmd));
	return (set_builtin(cmd), cmd);
}

char	**finish_split_tokens(char *cmd, t_pars_vars *v, char **temp)
{
	v->tokens = temp;
	v->tokens[v->tc] = NULL;
	return (free(cmd), v->tokens);
}
