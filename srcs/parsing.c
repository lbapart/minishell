/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/26 01:12:13 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_smplcmd
{
	char	*cmd;
	char	**args;
	char	*bultin;
	char	*input;
	char	*output;
} t_smplcmd;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	t_smplcmd	*smplcmd;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd;

void	*ft_realloc(void *ptr, size_t size)
{
	void *new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}

void ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	if (!dest || !src)
		return ;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

int is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int is_redirection(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	free_and_null(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

void	delete_parsed_cmd(char **cmd, size_t n)
{
	char *new_cmd;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	new_cmd = (char *)malloc(sizeof(char) * ft_strlen(*cmd) - n + 1);
	if (!new_cmd)
		return (free_and_null(cmd), NULL); // throw error here and free everything and exit
	while ((*cmd)[i] && i < n)
		i++;
	i++;
	if (!(*cmd)[i])
		return (free_and_null(cmd), NULL);
	while ((*cmd)[i])
	{
		new_cmd[j] = (*cmd)[i];
		i++;
		j++;
	}
	new_cmd[j] = '\0';
	free_and_null(cmd);
	*cmd = new_cmd;
}

int	set_in_quotes(char c, int *in_quotes, char **ptr)
{
	if (c == '\'' && *in_quotes != 2)
	{
		if (*in_quotes == 1)
			*in_quotes = 0;
		else
			*in_quotes = 1;
		(*ptr)++;
	}
	else if (c == '"' && *in_quotes != 1)
	{
		if (*in_quotes == 2)
			*in_quotes = 0;
		else
			*in_quotes = 2;
		(*ptr)++;
	}
	else
		return (0);
	return (1);
}

int	handle_token(char **start, char **end, char ***tokens, size_t *token_count)
{
	size_t	token_length;
	char	*token;

	if (*start != *end)
	{
		// found a token
		token_length = *end - *start;
		token = (char *)malloc(token_length + 1);
		if (!token)
			return (0); // throw error here and free everything and exit
		ft_strncpy(token, *start, token_length);
		token[token_length] = '\0';
		*tokens = (char **)ft_realloc(*tokens, (*token_count + 1) * sizeof(char *));
		if (!*tokens)
			return (0); // throw error here and free everything and exit
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
	handle_token(start, end, tokens, token_count);// redirection handling
	if (**end == *(*end + 1))
		*end += 2;
	else
		(*end)++;
	handle_token(start, end, tokens, token_count);
}

void	remove_unnecessary_quotes(char *str)
{
	char	*str_in;
	char	*str_out;
	int		in_single_quotes;
	int		in_double_quotes;

	str_in = str;
	str_out = str;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str_in) 
	{
		if (*str_in == '\'' && !in_double_quotes) 
		{
			in_single_quotes = !in_single_quotes;
			str_in++;
		} 
		else if (*str_in == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			str_in++;
		}
		else
			*(str_out++) = *(str_in++);
	}
	*str_out = '\0';
}

char	**split_command_to_tokens(char* cmd) 
{
	char	**tokens = NULL;
	size_t	token_count = 0;
	int	in_quotes = 0;  // 0 for no quotes, 1 for single quotes, 2 for double quotes
	char	*start = cmd;
	char	*end = cmd;

	while (*end) 
	{
		if (set_in_quotes(*end, &in_quotes, &end))
			;
		else if (is_whitespace(*end) && !in_quotes) 
			handle_token(&start, &end, &tokens, &token_count);
		else if (is_redirection(*end) && !in_quotes)
			handle_redirection(&start, &end, &tokens, &token_count);
		else 
			end++;
	}
	// last token
	handle_token(&start, &end, &tokens, &token_count);
	tokens = (char**)ft_realloc(tokens, (token_count + 1) * sizeof(char*));
	if (!tokens)
		return (NULL); // throw error here and free everything and exit
	tokens[token_count] = NULL;
	return (tokens);
}

// to do
void	put_tokens_to_struct(char **tokens)
{
	;
}

void	extract_cmd(char **cmd, size_t n)
{
	char *cmd_to_exec;
	size_t i;
	char **tokens;

	i = 0;
	cmd_to_exec = malloc(sizeof(char) * n + 1);
	if (!cmd_to_exec)
		return (free_and_null(cmd), NULL); // throw error here and free everything and exit
	while ((*cmd) && (*cmd)[i] && i < n)
	{
		cmd_to_exec[i] = (*cmd)[i];
		i++;
	}
	cmd_to_exec[i] = '\0';
	delete_parsed_cmd(cmd, n);
	tokens = split_command_to_tokens(cmd_to_exec);
	if (!tokens)
		return (NULL); // throw error here and free everything and exit
	i = 0;
	while (tokens && tokens[i])
		remove_unnecessary_quotes(tokens[i++]);
}

void parse_commands(char *cmd)
{
	size_t i;
	size_t last_pipe;
	t_cmd *cmds;
	int	is_open_single_quote;
	int	is_open_double_quote;

	i = 0;
	last_pipe = 0;
	is_open_single_quote = 0;
	is_open_double_quote = 0;
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' && !is_open_double_quote)
			is_open_single_quote = !is_open_single_quote;
		else if (cmd[i] == '"' && !is_open_single_quote)
			is_open_double_quote = !is_open_double_quote;
		else if (cmd[i] == '|' && cmd[i + 1] == '|' && !is_open_single_quote && !is_open_double_quote)
			return ; // throw an error here. || is not supported
		else if (cmd[i] == ';' || cmd[i] == '\\' && !is_open_single_quote && !is_open_double_quote)
			return ; // throw an error here. ; and \ are not supported
		else if (cmd[i] == '|' && !is_open_single_quote && !is_open_double_quote)
			extract_cmd(&cmd, i);
		i++;
	}
	extract_cmd(&cmd, i);
}
