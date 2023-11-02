/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:56:31 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/01 21:58:32 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
			return (free_dbl_ptr(*tokens), 0);
		ft_strncpy(token, *start, token_length);
		token[token_length] = '\0';
		temp = (char **)ft_realloc(*tokens,
				(*token_count + 1) * sizeof(char *));
		if (!temp)
			return (free_dbl_ptr(*tokens), 0);
		*tokens = temp;
		(*tokens)[*token_count] = token;
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
	t_vars	v;

	init_vars(&v, cmd);
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
	temp = (char **)ft_realloc(v.tokens, (v.tc + 1) * sizeof(char *));
	if (!temp)
		return (free(cmd), free_dbl_ptr(v.tokens), NULL);
	v.tokens = temp;
	v.tokens[v.tc] = NULL;
	return (free(cmd), v.tokens);
}

t_smplcmd	*put_tokens_to_struct(char **tokens, t_cmd *cmd)
{
	t_vars	v;

	init_vars(&v, NULL);
	v.smplcmd = init_simple_command();
	if (!v.smplcmd)
		return (free_dbl_ptr(tokens), free_structs(cmd), NULL);
	if (!check_and_put_path(tokens, v.smplcmd))
		return (free_everything(tokens, cmd, v.smplcmd), NULL);
	// if (v.smplcmd->path)
	// 	v.i++;
	while (tokens && tokens[v.i])
	{
		if (is_redirection(tokens[v.i][0]))
		{
			if (!add_redir_to_list(&v, tokens))
				return (free_everything(tokens, cmd, v.smplcmd), NULL);
			if (!tokens[v.i + 1])
				break ;
		}
		else
		{
			if (!put_token(&v, tokens))
				return (free_everything(tokens, cmd, v.smplcmd), NULL);
		}
	}
	return (v.smplcmd);
}
