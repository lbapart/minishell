/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 12:36:22 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// getenv should be replaced with custom function
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

void	set_cmds_to_null(t_cmd **cmds)
{
	*cmds = NULL;
}

void	set_hidden_quotes(char *cmd, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
	{
		if (cmd[i] == '\'')
			cmd[i] = HIDDEN_QUOTE;
		i++;
	}
}

void 	create_result_command(t_pars_vars *v, t_pars_vars *in_v)
{
	ft_strncpy(in_v->res, v->cmd_to_exec, in_v->i);
	ft_strcat(in_v->res, getenv(in_v->temp));
	set_hidden_quotes(in_v->res + in_v->i, ft_strlen(in_v->res + in_v->i));
	ft_strcat(in_v->res, v->cmd_to_exec + in_v->i + ft_strlen(in_v->temp) + 1);
	in_v->i = in_v->i + ft_strlen(getenv(in_v->temp));
	(free(v->cmd_to_exec), free(in_v->temp));
	v->cmd_to_exec = in_v->res;
}

void	replace_vars_with_values(char **str_cmd, t_pars_vars *v, t_cmd **cmds)
{
	t_pars_vars in_v;
	
	init_vars(&in_v, NULL, 0);
	while (v->cmd_to_exec[in_v.i])
	{
		if (v->cmd_to_exec[in_v.i] == '\'' && !in_v.is_open_single_quote)
			in_v.is_open_single_quote = !in_v.is_open_single_quote;
		else if (v->cmd_to_exec[in_v.i] == '\'' && in_v.is_open_single_quote)
			in_v.is_open_single_quote = !in_v.is_open_single_quote;
		if (v->cmd_to_exec[in_v.i] == '$' && !in_v.is_open_single_quote)
		{
			in_v.temp = get_var_name(v->cmd_to_exec + in_v.i + 1, cmds, str_cmd, v->cmd_to_exec);
			if (!in_v.temp && ++in_v.i)
				continue ;
			in_v.res = (char *)malloc(ft_strlen(v->cmd_to_exec) - (ft_strlen(in_v.temp) + 1)
					+ ft_strlen(getenv(in_v.temp)) + 1);
			if (!in_v.res)
				return (free(in_v.temp), free_dbl_ptr(v->tokens), free_and_null(str_cmd),
					free_structs(*cmds), malloc_err());
			create_result_command(v, &in_v);
		}
		in_v.i++;
	}
}

void	extract_cmd(char **str_cmd, size_t last_pipe, size_t n, t_cmd **cmds)
{
	t_pars_vars	v;

	init_vars(&v, NULL, 0);
	v.cmd_to_exec = (char *)malloc(sizeof(char) * (n - last_pipe) + 1);
	if (!v.cmd_to_exec)
		return (free_structs(*cmds), free(*str_cmd), malloc_err());
	copy_until_pipe(str_cmd, v.cmd_to_exec, last_pipe, n);
	replace_vars_with_values(str_cmd, &v, cmds);
	v.tokens = split_command_to_tokens(v.cmd_to_exec);
	if (!v.tokens)
		return (free_structs(*cmds), free(*str_cmd), malloc_err());
	if (!check_redir_tokens(v.tokens))
		return (free_structs(*cmds), set_cmds_to_null(cmds), free_dbl_ptr(v.tokens), redir_token_err());
	while (v.tokens && v.tokens[v.i])
		remove_unnecessary_quotes(v.tokens[v.i++]);
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

t_cmd	*parse_commands(char *cmd, int last_exit_code)
{
	t_pars_vars	v;

	init_vars(&v, NULL, last_exit_code);
	if (!cmd || !cmd[0])
		return (NULL);
	if (!check_unclosed_quotes(cmd))
		return (unclosed_quotes_err(), NULL);
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
	return (extract_cmd(&cmd, v.last_pipe, v.i, &v.cmds), finish_pars(v.cmds));
}

// int	main(void)
// {
// 	char *cmd;
// 	t_cmd *cmds;
// 	cmd = ft_strdup("$var");
// 	if (!cmd)
// 		return (0);
// 	cmds = parse_commands(cmd);
// 	//printf("saassasaas\n");
// 	print_commands(cmds);
// 	free_structs(cmds);
// 	free(cmd);
// 	return (0);
// }
