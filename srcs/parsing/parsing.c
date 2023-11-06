/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 18:51:09 by lbapart          ###   ########.fr       */
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

void 	create_result_command(t_pars_vars *v, t_pars_vars *in_v, char *var_value)
{
	ft_strncpy(in_v->res, v->cmd_to_exec, in_v->i);
	ft_strcat(in_v->res, var_value);
	set_hidden_quotes(in_v->res + in_v->i, ft_strlen(in_v->res + in_v->i));
	ft_strcat(in_v->res, v->cmd_to_exec + in_v->i + ft_strlen(in_v->temp) + 1);
	in_v->i = in_v->i + ft_strlen(var_value);
	if (ft_strcmp(in_v->temp, "?") == 0)
		free(var_value);
	(free(v->cmd_to_exec), free(in_v->temp));
	v->cmd_to_exec = in_v->res;
}

char	*get_var_value(char *key, t_shell *shell, t_cmd **cmds, char **str_cmd)
{
	t_vars *temp;
	char	*res;

	if (ft_strcmp(key, "?") == 0)
	{
		res = ft_itoa(shell->last_exit_code);
		if (!res)
			return (free(key), free_structs(cmds), free_and_null(str_cmd),
				free_all_envs(&shell->env), free_all_envs(&shell->exported_vars),
				malloc_err(), NULL);
		return (res);
	}
	temp = shell->env;
	temp = find_key(key, temp);
	if (temp)
		return (temp->value);
	temp = shell->exported_vars;
	temp = find_key(key, temp);
	if (temp)
		return (temp->value);
	return (NULL);
}

void	free_if_question(char *key, char *var_value)
{
	if (ft_strcmp(key, "?") == 0)
		free(var_value);
	free(key);
}

// possible leak in case var_value is ? if malloc in line 105 fails
void	replace_vars_with_values(char **str_cmd, t_pars_vars *v, t_cmd **cmds, t_shell *shell)
{
	t_pars_vars in_v;
	char		*var_value;
	
	init_vars(&in_v, NULL);
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
			var_value = get_var_value(in_v.temp, shell, cmds, str_cmd);
			in_v.res = (char *)malloc(ft_strlen(v->cmd_to_exec) - (ft_strlen(in_v.temp) + 1)
					+ ft_strlen(var_value) + 1);
			if (!in_v.res)
				return (free_if_question(in_v.temp, var_value), free_dbl_ptr(v->tokens), free_and_null(str_cmd),
					free_structs(cmds), free_all_envs(&shell->env), free_all_envs(&shell->exported_vars), malloc_err());
			create_result_command(v, &in_v, var_value);
		}
		in_v.i++;
	}
}

void	extract_cmd(char **str_cmd, t_pars_vars *out_v, t_shell *shell) // size_t last_pipe, size_t n, t_cmd **cmds)
{
	t_pars_vars	v;

	init_vars(&v, NULL);
	v.cmd_to_exec = (char *)malloc(sizeof(char) * (out_v->i - out_v->last_pipe) + 1);
	if (!v.cmd_to_exec)
		return (free_structs(&out_v->cmds), free(*str_cmd), malloc_err());
	copy_until_pipe(str_cmd, v.cmd_to_exec, out_v->last_pipe, out_v->i);
	replace_vars_with_values(str_cmd, &v, &out_v->cmds, shell);
	v.tokens = split_command_to_tokens(v.cmd_to_exec);
	if (!v.tokens)
		return (free_structs(&out_v->cmds), free(*str_cmd), malloc_err());
	if (!check_redir_tokens(v.tokens))
		return (free_structs(&out_v->cmds), free_dbl_ptr(v.tokens), redir_token_err());
	while (v.tokens && v.tokens[v.i])
		remove_unnecessary_quotes(v.tokens[v.i++]);
	v.smplcmd = put_tokens_to_struct(v.tokens, out_v->cmds);
	if (!v.smplcmd)
		return (free(*str_cmd), malloc_err());
	free_dbl_ptr(v.tokens);
	v.new_cmd = init_new_cmd(shell);
	if (!v.new_cmd)
		return (free_structs(&out_v->cmds), free(*str_cmd), malloc_err());
	v.new_cmd->smplcmd = v.smplcmd;
	lst_cmd_add_back(&out_v->cmds, v.new_cmd);
}

t_cmd	*parse_commands(char *cmd, t_shell *shell)
{
	t_pars_vars	v;

	init_vars(&v, NULL);
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
			return (free_structs(&v.cmds), double_pipe_err(), NULL);
		else if (is_unsupported_char(cmd[v.i]) && !v.is_open_single_quote && !v.is_open_double_quote)
			return (free_structs(&v.cmds), unsupported_char_err(cmd[v.i]), NULL);
		else if (cmd[v.i] == '|' && !v.is_open_single_quote && !v.is_open_double_quote)
		{
			extract_cmd(&cmd, &v, shell);
			v.last_pipe = v.i + 1;
		}
		v.i++;
	}
	return (extract_cmd(&cmd, &v, shell), finish_pars(v.cmds));
}
