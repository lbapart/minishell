/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/25 23:28:08 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_smplcmd
{
	char	*cmd;
	char	**args;
	char	*bultin;
	char	*redirect;
} t_smplcmd;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	char *input;
	char *output;
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


// need to be splitted into smaller functions
// init cmd for vars??))))
// mb here also handle pipes
// in_quotes replace with is_single_quote and is_double_quote?? - guess no
char	**split_to_tokens_command(char* cmd) {
	char	**tokens = NULL;
	size_t	token_count = 0;
	int	in_quotes = 0;  // 0 for no quotes, 1 for single quotes, 2 for double quotes
	char	*start = cmd;
	char	*end = cmd;

	while (*end) 
	{
		if (*end == '\'' && in_quotes != 2) 
		{
			in_quotes = (in_quotes == 1) ? 0 : 1;
			end++;
		} 
		else if (*end == '"' && in_quotes != 1) 
		{
			in_quotes = (in_quotes == 2) ? 0 : 2;
			end++;
		}
		// rewrite part above to handle quotes
		else if (is_whitespace(*end) && !in_quotes) 
		{
			if (start != end) 
			{
				// found a token
				size_t token_length = end - start;
				char* token = (char*)malloc(token_length + 1);
				if (!token) 
					return (NULL); // throw error here and free everything and exit
				ft_strncpy(token, start, token_length);
				token[token_length] = '\0';
				tokens = (char**)ft_realloc(tokens, (token_count + 1) * sizeof(char*));
				if (!tokens) 
					return (NULL); // throw error here and free everything and exit
				tokens[token_count] = token;
				token_count++;
			}
			while (is_whitespace(*end))
				end++;
			start = end;
		}
		// put part above in a function
		else if (is_redirection(*end) && !in_quotes) {
			// redirection handling
		} 
		else 
			end++;
	}
	// last token
	if (start != end) {
		size_t token_length = end - start;
		char* token = (char*)malloc(token_length + 1);
		if (!token)
			return (NULL); // throw error here and free everything and exit
		ft_strncpy(token, start, token_length);
		token[token_length] = '\0';
		tokens = (char**)ft_realloc(tokens, (token_count + 1) * sizeof(char*));
		if (!tokens)
			return (NULL); // throw error here and free everything and exit
		tokens[token_count] = token;
		token_count++;
	}
	tokens = (char**)ft_realloc(tokens, (token_count + 1) * sizeof(char*));
	if (!tokens)
		return (NULL); // throw error here and free everything and exit
	tokens[token_count] = NULL;

	return tokens;
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
	tokens = split_to_tokens_command(cmd_to_exec);
	
}

void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
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
		else if (cmd[i] == '|' && !is_open_single_quote && !is_open_double_quote)
			extract_cmd(&cmd, i);
		i++;
	}
	extract_cmd(&cmd, i);
}
