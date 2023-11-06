/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:56:31 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 21:17:33 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_token(char **start, char **end, char ***tokens, size_t *token_count)
{
	size_t	token_length;
	char	*token;
	char	**temp;

	if (*start != *end)
	{
		token_length = *end - *start;
		token = (char *)malloc(token_length + 1);
		if (!token)
			return (0);
		ft_strncpy(token, *start, token_length);
		token[token_length] = '\0';
		temp = (char **)ft_realloc(*tokens,
				(*token_count + 2) * sizeof(char *), *token_count * sizeof(char *));
		if (!temp)
			return (free(token), 0);
		*tokens = temp;
		(*tokens)[*token_count] = token;
		(*tokens)[*token_count + 1] = NULL;
		(*token_count)++;
	}
	while (is_whitespace(**end))
		(*end)++;
	*start = *end;
	return (1);
}

int	handle_redirection(char **start, char **end,
					char ***tokens, size_t *token_count)
{
	if (!handle_token(start, end, tokens, token_count))
		return (0);
	if (**end == *(*end + 1))
		*end += 2;
	else
		(*end)++;
	if (!handle_token(start, end, tokens, token_count))
		return (0);
	return (1);
}

char	**split_command_to_tokens(char *cmd)
{
	char	**temp;
	t_pars_vars	v;

	init_vars(&v, cmd, NULL);
	while (*v.end)
	{
		if (set_in_quotes_flag(*v.end, &v.in_quotes, &v.end))
			;
		else if (is_whitespace(*v.end) && !v.in_quotes)
			v.hr = handle_token(&v.start, &v.end, &v.tokens, &v.tc);
		else if (is_redirection(*v.end) && !v.in_quotes)
			v.hr = handle_redirection(&v.start, &v.end, &v.tokens, &v.tc);
		else
			v.end++;
		if (!v.hr)
			return (free(cmd), free_dbl_ptr(v.tokens), NULL);
	}
	if (!handle_token(&v.start, &v.end, &v.tokens, &v.tc))
		return (free(cmd), free_dbl_ptr(v.tokens), NULL);
	temp = (char **)ft_realloc(v.tokens, (v.tc + 1) * sizeof(char *), v.tc * sizeof(char *));
	if (!temp)
		return (free(cmd), free_dbl_ptr(v.tokens), NULL);
	v.tokens = temp;
	v.tokens[v.tc] = NULL;
	return (free(cmd), v.tokens);
}

int	put_path(t_smplcmd *smplcmd)
{
	if (!smplcmd->args)
		return (1);
	else
	{
		smplcmd->path = ft_strdup(smplcmd->args[0]);
		if (!smplcmd->path)
			return (0);		
	}
	return (1);
}

t_smplcmd	*put_tokens_to_struct(char **tokens, t_cmd *cmd)
{
	t_pars_vars	v;

	init_vars(&v, NULL, NULL);
	v.smplcmd = init_simple_command();
	if (!v.smplcmd)
		return (free_dbl_ptr(tokens), free_structs(&cmd), NULL);
	while (tokens && tokens[v.i])
	{
		if (is_redirection(tokens[v.i][0]))
		{
			if (!add_redir_to_list(&v, tokens))
				return (free_everything(tokens, cmd, v.smplcmd), NULL);
		}
		else
		{
			if (!put_token(&v, tokens))
				return (free_everything(tokens, cmd, v.smplcmd), NULL);
		}
	}
	if (!put_path(v.smplcmd))
		return (free_everything(tokens, cmd, v.smplcmd), NULL);
	return (v.smplcmd);
}
