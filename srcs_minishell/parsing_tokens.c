/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:56:31 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 22:09:04 by lbapart          ###   ########.fr       */
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
		// found a token
		token_length = *end - *start;
		token = (char *)malloc(token_length + 1);
		if (!token)
			return (free_dbl_ptr(*tokens), 0); // throw error here and free everything and exit
		ft_strncpy(token, *start, token_length);
		token[token_length] = '\0';
		temp = (char **)ft_realloc(*tokens, (*token_count + 1) * sizeof(char *));
		if (!temp)
			return (free_dbl_ptr(*tokens), 0); // throw error here and free everything and exit
		*tokens = temp;
		(*tokens)[*token_count] = token;
		(*token_count)++;
	}
	while (is_whitespace(**end))
		(*end)++;
	*start = *end;
	return (1);
}

int	handle_redirection(char **start, char **end, char ***tokens, size_t *token_count)
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
	char	**tokens = NULL;
	size_t	token_count = 0;
	int		in_quotes = 0;  // 0 for no quotes, 1 for single quotes, 2 for double quotes
	char	*start = cmd;
	char	*end = cmd;
	char	**temp;
	int		handling_result;

	handling_result = 1;
	while (*end) 
	{
		if (set_in_quotes_flag(*end, &in_quotes, &end))
			;
		else if (is_whitespace(*end) && !in_quotes) 
			handling_result = handle_token(&start, &end, &tokens, &token_count);
		else if (is_redirection(*end) && !in_quotes)
			handling_result = handle_redirection(&start, &end, &tokens, &token_count);
		else 
			end++;
		if (!handling_result)
			return (free(cmd), free_dbl_ptr(tokens), NULL);
	}
	// last token
	if (!handle_token(&start, &end, &tokens, &token_count))
		return (free(cmd), free_dbl_ptr(tokens), NULL);
	temp = (char **)ft_realloc(tokens, (token_count + 1) * sizeof(char*));
	if (!temp)
		return (free(cmd), free_dbl_ptr(tokens), NULL); // throw error here and free everything and exit
	tokens = temp;
	tokens[token_count] = NULL;
	return (free(cmd), tokens);
}

t_smplcmd	*put_tokens_to_struct(char **tokens, t_cmd *cmd)
{
	size_t i;
	size_t j;
	t_smplcmd *smplcmd;
	t_redirection *redir;
	char **temp;

	smplcmd = init_simple_command(cmd);
	i = 0;
	j = 0;
	check_and_put_path(tokens, smplcmd);
	if (smplcmd->path)
		i++;
	while (tokens && tokens[i])
	{
		if (is_redirection(tokens[i][0]))
		{
			redir = init_redir();
			if (tokens[i][0] == '<' && tokens[i][1] == '<')
				redir->type = 2;
			else if (tokens[i][0] == '>' && tokens[i][1] == '>')
				redir->type = 4;
			else if (tokens[i][0] == '<')
				redir->type = 1;
			else if (tokens[i][0] == '>')
				redir->type = 3;
			redir->file = ft_strdup(tokens[i + 1]);
			if (!redir->file)
				return (free_dbl_ptr(tokens), free_smplcmd(smplcmd), free_structs(cmd), NULL); // throw error here and free everything and exit
			i += 2;
			lst_redir_add_back(&smplcmd->redir, redir);
			if (!tokens[i + 1])
				break ;
		}
		else
		{
			temp = (char **)ft_realloc(smplcmd->args, (j + 2) * sizeof(char *));
			if (!temp)
				return (free_dbl_ptr(tokens), free_smplcmd(smplcmd), free_structs(cmd), NULL); // throw error here and free everything and exit
			smplcmd->args = temp;
			smplcmd->args[j] = ft_strdup(tokens[i]);
			if (!smplcmd->args[j])
				return (free_dbl_ptr(tokens), free_smplcmd(smplcmd), free_structs(cmd), NULL); // throw error here and free everything and exit
			smplcmd->args[j + 1] = NULL;
			i++;
			j++;
		}
	}
	return (smplcmd);
}