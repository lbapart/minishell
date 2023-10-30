/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/30 21:01:04 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// getenv should be replaced with custom function
void	create_result_token(t_vars *v, char **tokens, t_cmd *t_cmd, char **cmd)
{
	v->temp = get_var_name(tokens[v->i] + v->j + 1, tokens, t_cmd, cmd);
	v->res = (char *)malloc(ft_strlen(tokens[v->i]) - (ft_strlen(v->temp) + 1)
			+ ft_strlen(getenv(v->temp)) + 1);
	if (!v->res)
		return (free(v->temp), free_dbl_ptr(tokens), free_and_null(cmd),
			free_structs(t_cmd), malloc_err());
	ft_strncpy(v->res, tokens[v->i], v->j);
	ft_strcat(v->res, getenv(v->temp));
	ft_strcat(v->res, tokens[v->i] + v->j + ft_strlen(v->temp) + 1);
	v->j = v->j + ft_strlen(getenv(v->temp));
	(free(tokens[v->i]), free(v->temp));
	tokens[v->i] = v->res;
}

void	copy_until_pipe(char **str_cmd, char *cmd_to_exec,
					size_t last_pipe, size_t n)
{
	size_t	i;

	i = 0;
	while ((*str_cmd) && (*str_cmd)[i + last_pipe] && i + last_pipe < n)
	{
		cmd_to_exec[i] = (*str_cmd)[i + last_pipe];
		i++;
	}
	cmd_to_exec[i] = '\0';
}

void	replace_vars_with_values(char **tokens, char **cmd, t_cmd *t_cmd)
{
	t_vars	v;

	init_vars(&v, NULL);
	while (tokens && tokens[v.i])
	{
		v.j = 0;
		v.is_open_single_quote = 0;
		while (tokens[v.i][v.j])
		{
			if (tokens[v.i][v.j] == '\'' && !v.is_open_single_quote)
				v.is_open_single_quote = !v.is_open_single_quote;
			else if (tokens[v.i][v.j] == '\'' && v.is_open_single_quote)
				v.is_open_single_quote = !v.is_open_single_quote;
			if (tokens[v.i][v.j] == '$' && !v.is_open_single_quote)
				create_result_token(&v, tokens, t_cmd, cmd);
			v.j++;
		}
		v.i++;
	}
}

void	extract_cmd(char **str_cmd, size_t last_pipe, size_t n, t_cmd **cmds)
{
	t_vars	v;

	init_vars(&v, NULL);
	v.cmd_to_exec = (char *)malloc(sizeof(char) * (n - last_pipe) + 1);
	if (!v.cmd_to_exec)
		return (free_structs(*cmds), free(*str_cmd), malloc_err());
	copy_until_pipe(str_cmd, v.cmd_to_exec, last_pipe, n);
	v.tokens = split_command_to_tokens(v.cmd_to_exec);
	if (!v.tokens)
		return (free_structs(*cmds), free(*str_cmd), malloc_err());
	v.i = 0;
	while (v.tokens && v.tokens[v.i])
	{
		replace_vars_with_values(v.tokens, str_cmd, *cmds);
		remove_unnecessary_quotes(v.tokens[v.i++]);
	}
	v.smplcmd = put_tokens_to_struct(v.tokens, *cmds);
	if (!v.smplcmd)
		return (free(*str_cmd), malloc_err());
	free_dbl_ptr(v.tokens);
	v.new_cmd = init_new_cmd();
	if (!v.new_cmd)
		return (free_structs(*cmds), free(*str_cmd), malloc_err());
	v.new_cmd->smplcmd = v.smplcmd;
	lst_cmd_add_back(cmds, v.new_cmd);
}

t_cmd	*parse_commands(char *cmd)
{
	t_vars	v;

	init_vars(&v, NULL);
	if (!check_unclosed_quotes(cmd))
		return (unclosed_quotes_err(), NULL);
	if (!cmd || !cmd[0])
		return (NULL);
	while (cmd && cmd[v.i])
	{
		if (cmd[v.i] == '\'' && !v.is_open_double_quote)
			v.is_open_single_quote = !v.is_open_single_quote;
		else if (cmd[v.i] == '"' && !v.is_open_single_quote)
			v.is_open_double_quote = !v.is_open_double_quote;
		else if (cmd[v.i] == '|' && cmd[v.i + 1] == '|' && !v.is_open_single_quote && !v.is_open_double_quote)
			return (free_structs(v.cmds), double_pipe_err(), NULL);
		else if (is_unsupported_char(cmd[v.i]) && !v.is_open_single_quote && !v.is_open_double_quote)
			return (free_structs(v.cmds), unsupported_char_err(cmd[v.i]), NULL);
		else if (cmd[v.i] == '|' && !v.is_open_single_quote && !v.is_open_double_quote)
		{
			extract_cmd(&cmd, v.last_pipe, v.i, &v.cmds);
			v.last_pipe = v.i + 1;
		}
		v.i++;
	}
	return (extract_cmd(&cmd, v.last_pipe, v.i, &v.cmds), v.cmds);
}

int	main(void)
{
	char *cmd;
	t_cmd *cmds;
	cmd = ft_strdup("echo doesntexist | ./bin echo ghi > file1 > file2abc | echo jkl");
	if (!cmd)
		return (0);
	cmds = parse_commands(cmd);
	//printf("saassasaas\n");
	print_commands(cmds);
	//execve("/bin/ls", (char *[]){NULL}, NULL);
	//printf("%s\n", getenv("?"));
	free_structs(cmds);
	free(cmd);
	return (0);
}