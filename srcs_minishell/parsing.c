/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/29 00:44:52 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

// probably better to return simple command here
void	extract_cmd(char **str_cmd, size_t last_pipe, size_t n, t_cmd **cmds)
{
	char *cmd_to_exec;
	size_t i;
	t_cmd *new_cmd;
	t_smplcmd *smplcmd;
	char **tokens;

	i = 0;
	cmd_to_exec = (char *)malloc(sizeof(char) * (n - last_pipe) + 1);
	if (!cmd_to_exec)
		return (free_structs(*cmds), free(*str_cmd), exit(MALLOCEXIT));
	while ((*str_cmd) && (*str_cmd)[i + last_pipe] && i + last_pipe < n)
	{
		cmd_to_exec[i] = (*str_cmd)[i + last_pipe];
		i++;
	}
	cmd_to_exec[i] = '\0';
	tokens = split_command_to_tokens(cmd_to_exec);
	if (!tokens)
		return (free_structs(*cmds), free(*str_cmd), exit(MALLOCEXIT));
	i = 0;
	while (tokens && tokens[i])
		(replace_vars_with_values(tokens, str_cmd, *cmds), remove_unnecessary_quotes(tokens[i++]));
	smplcmd = put_tokens_to_struct(tokens, *cmds);
	if (!smplcmd)
		return (free(*str_cmd), exit(MALLOCEXIT));
	free_dbl_ptr(tokens);
	new_cmd = init_new_cmd();
	if (!new_cmd)
		return (free_structs(*cmds), free(*str_cmd), exit(MALLOCEXIT));
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
	if (!check_unclosed_quotes(cmd) || !cmd || !cmd[0])
		return (free(cmd), NULL); // throw error here
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' && !is_open_double_quote)
			is_open_single_quote = !is_open_single_quote;
		else if (cmd[i] == '"' && !is_open_single_quote)
			is_open_double_quote = !is_open_double_quote;
		else if (cmd[i] == '|' && cmd[i + 1] == '|' && !is_open_single_quote && !is_open_double_quote)
			return (free(cmd), free_structs(cmds), NULL); // throw an error here. || is not supported
		else if ((cmd[i] == ';' || cmd[i] == '\\') && !is_open_single_quote && !is_open_double_quote)
			return (free(cmd), free_structs(cmds), NULL); // throw an error here. ; and \ are not supported
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
	cmd = ft_strdup("echo abcin > | echo def | ./bin echo ghi | echo jkl | echo mno");
	if (!cmd)
		return (0);
	cmds = parse_commands(cmd);
	print_commands(cmds);
	printf("%s\n", getenv("?"));
	free_structs(cmds);
	free(cmd);
	return (0);
}