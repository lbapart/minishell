/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:56:22 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/30 20:44:59 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	add_redir_to_list(t_vars *v, char **tokens)
{
	v->redir = init_redir();
	if (!v->redir)
		return (0);
	if (tokens[v->i][0] == '<' && tokens[v->i][1] == '<')
		v->redir->type = 2;
	else if (tokens[v->i][0] == '>' && tokens[v->i][1] == '>')
		v->redir->type = 4;
	else if (tokens[v->i][0] == '<')
		v->redir->type = 1;
	else if (tokens[v->i][0] == '>')
		v->redir->type = 3;
	v->redir->file = ft_strdup(tokens[v->i + 1]);
	if (!v->redir->file)
		return (free(v->redir), 0);
	v->i += 2;
	lst_redir_add_back(&v->smplcmd->redir, v->redir);
	return (1);
}

int	put_token(t_vars *v, char **tokens)
{
	char	**temp;

	temp = (char **)ft_realloc(v->smplcmd->args, (v->j + 2) * sizeof(char *));
	if (!temp)
		return (free_dbl_ptr(v->smplcmd->args), 0);
	v->smplcmd->args = temp;
	v->smplcmd->args[v->j] = ft_strdup(tokens[v->i]);
	if (!v->smplcmd->args[v->j])
		return (free_dbl_ptr(v->smplcmd->args), 0);
	v->smplcmd->args[v->j + 1] = NULL;
	v->i++;
	v->j++;
	return (1);
}
