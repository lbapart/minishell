/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 17:48:57 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void print_commands(t_cmd *cmds)
{
	t_cmd *tmp;
	t_smplcmd *smplcmd;
	t_redirection *temp;
	size_t i;
	size_t count = 1;

	tmp = cmds;
	while (tmp)
	{
		printf("--------------------\n");
		printf("cmd: %s\n", tmp->cmd);
		smplcmd = tmp->smplcmd;
		printf("path: %s\n", smplcmd->path);
		i = 0;
		while (smplcmd->args && smplcmd->args[i])
		{
			printf("arg %zu: %s\n", i, smplcmd->args[i]);
			i++;
		}
		temp = smplcmd->redir;
		while (temp)
		{
			printf("redir type: %d\n", temp->type);
			printf("redir file: %s\n", temp->file);
			temp = temp->next;
		}
		tmp = tmp->next;
		printf("--------------------\n");
	}
}

// to libft?
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

void	ft_strcat(char *dest, const char *src)
{
	size_t i;
	size_t j;

	i = 0;
	j = ft_strlen(dest);
	while (src && src[i])
		dest[j++] = src[i++];
	dest[j] = '\0';
}

// to libft?
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


// is_whitespace and is_redirection and free_and_null to utils.c
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


// all these fucking functions need to be tested. to think about free and exit also
int	set_in_quotes_flag(char c, int *in_quotes, char **ptr)
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
	int		in_quotes = 0;  // 0 for no quotes, 1 for single quotes, 2 for double quotes
	char	*start = cmd;
	char	*end = cmd;

	while (*end) 
	{
		if (set_in_quotes_flag(*end, &in_quotes, &end))
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

int	check_unclosed_quotes(char	*cmd)
{
	int		in_quotes;

	in_quotes = 0;
	while (*cmd)
	{
		if (*cmd == '\'' && in_quotes != 2)
		{
			if (in_quotes == 1)
				in_quotes = 0;
			else
				in_quotes = 1;
		}
		else if (*cmd == '"' && in_quotes != 1)
		{
			if (in_quotes == 2)
				in_quotes = 0;
			else
				in_quotes = 2;
		}
		cmd++;
	}
	if (in_quotes)
		return (0);
	return (1);
}

void	check_and_put_path(char **tokens, t_smplcmd *smplcmd)
{
	if (tokens && tokens[0] && tokens[0][0] == '/' || tokens[0][0] == '.')
	{
		smplcmd->path = ft_strdup(tokens[0]);
		if (!smplcmd->path)
			return ; // throw error here and free everything and exit
	}
	else
	{
		smplcmd->path = NULL;
	}
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

t_smplcmd	*put_tokens_to_struct(char **tokens, t_cmd *cmd)
{
	size_t i;
	size_t j;
	t_smplcmd *smplcmd;
	t_redirection *redir;

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
				return (NULL); // throw error here and free everything and exit
			i += 2;
			lst_redir_add_back(&smplcmd->redir, redir);
			if (!tokens[i + 1])
				break ;
		}
		else
		{
			smplcmd->args = (char **)ft_realloc(smplcmd->args, (j + 2) * sizeof(char *));
			if (!smplcmd->args)
				return (NULL); // throw error here and free everything and exit
			smplcmd->args[j] = ft_strdup(tokens[i]);
			if (!smplcmd->args[j])
				return (NULL); // throw error here and free everything and exit
			smplcmd->args[j + 1] = NULL;
			i++;
			j++;
		}
	}
	return (smplcmd);
}

int	is_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*get_var_name(char *var, char **tokens, t_cmd *t_cmd, char **cmd)
{
	size_t i;
	char *res;

	i = 0;
	while (var[i] && is_var_char(var[i]))
		i++;
	if (i == 0)
		return (NULL);
	res = (char *)malloc(i + 1);
	if (!res)
		return (NULL); // throw error here and free everything and exit
	ft_strncpy(res, var, i);
	return (res);
}

void	replace_vars_with_values(char **tokens, char **cmd, t_cmd *t_cmd)
{
	size_t i;
	size_t j;
	char *temp;
	char *res;
	int	is_open_single_quote;

	i = 0;
	while (tokens && tokens[i])
	{
		j = 0;
		is_open_single_quote = 0;
		while (tokens[i][j])
		{
			if (tokens[i][j] == '\'' && !is_open_single_quote)
				is_open_single_quote = !is_open_single_quote;
			else if (tokens[i][j] == '\'' && is_open_single_quote)
				is_open_single_quote = !is_open_single_quote;
			if (tokens[i][j] == '$' && !is_open_single_quote)
			{
				temp = get_var_name(tokens[i] + j + 1, tokens, t_cmd, cmd);
				res = (char *)malloc(ft_strlen(tokens[i]) - (ft_strlen(temp) + 1) + ft_strlen(getenv(temp)) + 1);
				if (!res)
					return ; // throw error here and free everything and exit
				ft_strncpy(res, tokens[i], j);
				ft_strcat(res, getenv(temp));
				ft_strcat(res, tokens[i] + j + ft_strlen(temp) + 1);
				j = j + ft_strlen(getenv(temp));
				(free(tokens[i]), free(temp));
				tokens[i] = res;
			}
			j++;
		}
		i++;
	}
}

t_cmd	*init_new_cmd(char *cmd, t_cmd *cmds)
{
	t_cmd *new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL); // throw error here and free everything and exit
	new_cmd->cmd = ft_strdup(cmd);
	if (!new_cmd->cmd)
		return (NULL); // throw error here and free everything and exit
	new_cmd->smplcmd = NULL;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->signal_received = 0;
	return (new_cmd);
}

// probably better to return simple command here
void	extract_cmd(char **str_cmd, size_t last_pipe, size_t n, t_cmd **cmds)
{
	char *cmd_to_exec;
	size_t i;
	t_cmd *new_cmd;
	t_smplcmd *smplcmd;
	char **tokens;

	i = 0;
	cmd_to_exec = malloc(sizeof(char) * (n - last_pipe) + 1);
	if (!cmd_to_exec)
		return (free_and_null(str_cmd)); // throw error here and free everything and exit
	while ((*str_cmd) && (*str_cmd)[i + last_pipe] && i < n)
	{
		cmd_to_exec[i] = (*str_cmd)[i + last_pipe];
		i++;
	}
	cmd_to_exec[i] = '\0';
	tokens = split_command_to_tokens(cmd_to_exec);
	if (!tokens)
		return ; // throw error here and free everything and exit
	i = 0;
	while (tokens && tokens[i])
	{
		replace_vars_with_values(tokens, str_cmd, *cmds);
		remove_unnecessary_quotes(tokens[i++]);
	}
	smplcmd = put_tokens_to_struct(tokens, *cmds);
	new_cmd = init_new_cmd(cmd_to_exec, *cmds);
	if (!new_cmd)
		return ; // throw error here and free everything and exit
	new_cmd->smplcmd = smplcmd;
	lst_cmd_add_back(cmds, new_cmd);
}

t_cmd *parse_commands(char *cmd)
{
	size_t i;
	size_t last_pipe;
	t_cmd *cmds;
	int	is_open_single_quote;
	int	is_open_double_quote;

	i = 0;
	is_open_single_quote = 0;
	is_open_double_quote = 0;
	last_pipe = 0;
	cmds = NULL;
	if (!check_unclosed_quotes(cmd))
	{
		free(cmd);
		return (NULL); // throw error here
	}
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' && !is_open_double_quote)
			is_open_single_quote = !is_open_single_quote;
		else if (cmd[i] == '"' && !is_open_single_quote)
			is_open_double_quote = !is_open_double_quote;
		else if (cmd[i] == '|' && cmd[i + 1] == '|' && !is_open_single_quote && !is_open_double_quote)
			return (NULL); // throw an error here. || is not supported
		else if (cmd[i] == ';' || cmd[i] == '\\' && !is_open_single_quote && !is_open_double_quote)
			return (NULL); // throw an error here. ; and \ are not supported
		else if (cmd[i] == '|' && !is_open_single_quote && !is_open_double_quote)
		{
			extract_cmd(&cmd, last_pipe, i, &cmds);
			last_pipe = i + 1;
		}
		i++;
	}
	extract_cmd(&cmd, last_pipe, i, &cmds);
	return (cmds);
}

int	main()
{
	char *cmd;
	t_cmd *cmds;
	cmd = ft_strdup("./bin ec'ho' $HOME | cat -e > file << input");
	if (!cmd)
		return (0);
	cmds = parse_commands(cmd);
	print_commands(cmds);
	return (0);
}