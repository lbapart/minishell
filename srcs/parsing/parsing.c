/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 17:21:30 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_result_command(t_pars_vars *v, t_pars_vars *in_v,
				char *var_value)
{
	ft_strncpy(in_v->res, v->cmd_to_exec, in_v->i);
	ft_strcat(in_v->res, var_value);
	set_hidden_quotes(in_v->res + in_v->i, ft_strlen(in_v->res + in_v->i));
	ft_strcat(in_v->res, v->cmd_to_exec + in_v->i + ft_strlen(in_v->temp) + 1);
	in_v->i = in_v->i + ft_strlen(var_value) - 1;
	if (ft_strcmp(in_v->temp, "?") == 0)
		free(var_value);
	(free(v->cmd_to_exec), free(in_v->temp));
	v->cmd_to_exec = in_v->res;
}

int	extract_cmd(char **str_cmd, t_pars_vars *p, t_shell *shell)
{
	t_pars_vars	v;

	init_vars(&v, NULL, shell);
	v.cmd_to_exec = (char *)malloc(sizeof(char) * (p->i - p->last_pipe) + 1);
	if (!v.cmd_to_exec)
		return (free_extract_cmd(&p->cmds, str_cmd, shell), malloc_err(), 0);
	copy_until_pipe(str_cmd, v.cmd_to_exec, p->last_pipe, p->i);
	replace_vars_with_values(str_cmd, &v, &p->cmds, shell);
	v.tokens = split_command_to_tokens(v.cmd_to_exec);
	if (!v.tokens)
		return (free_extract_cmd(&p->cmds, str_cmd, shell), malloc_err(), 0);
	if (!check_redir_tokens(v.tokens))
		return (free_extract_cmd_2(&p->cmds, &v), redir_token_err(shell), 0);
	while (v.tokens && v.tokens[v.i])
		remove_unnecessary_quotes(v.tokens[v.i++]);
	v.smplcmd = put_tokens_to_struct(v.tokens, p->cmds);
	if (!v.smplcmd)
		return (free_extract_cmd_3(str_cmd, shell), malloc_err(), 0);
	free_dbl_ptr(v.tokens);
	v.new_cmd = init_new_cmd(shell);
	if (!v.new_cmd)
		return (free_ext_cmd4_and_err(&p->cmds, v.smplcmd, str_cmd, shell), 0);
	v.new_cmd->smplcmd = v.smplcmd;
	lst_cmd_add_back(&p->cmds, v.new_cmd);
	return (1);
}

int	parse_init_check(char *cmd, t_pars_vars *v, t_shell *shell)
{
	if (!cmd || !cmd[0])
		return (0);
	if (!check_unclosed_quotes(cmd))
		return (unclosed_quotes_err(shell), 0);
	init_vars(v, NULL, shell);
	return (1);
}

void	set_prev_cmds(t_cmd *cmds)
{
	t_cmd	*temp;

	temp = cmds;
	while (temp)
	{
		if (temp->next)
			temp->next->prev = temp;
		temp = temp->next;
	}
}

t_cmd	*parse_commands(char *cmd, t_shell *shell)
{
	t_pars_vars	v;

	if (!parse_init_check(cmd, &v, shell))
		return (NULL);
	while (cmd && cmd[v.i])
	{
		if (cmd[v.i] == '\'' && !v.iodq)
			v.iosq = !v.iosq;
		else if (cmd[v.i] == '"' && !v.iosq)
			v.iodq = !v.iodq;
		else if (cmd[v.i] == '|' && cmd[v.i + 1] == '|' && !v.iosq && !v.iodq)
			return (free_structs(&v.cmds), double_pipe_err(shell), NULL);
		else if (is_unsupported_char(cmd[v.i]) && !v.iosq && !v.iodq)
			return (free_structs(&v.cmds), char_err(cmd[v.i], shell), NULL);
		else if (cmd[v.i] == '|' && !v.iosq && !v.iodq)
		{
			if (!extract_cmd(&cmd, &v, shell))
				return (NULL);
			v.last_pipe = v.i + 1;
		}
		v.i++;
	}
	if (!extract_cmd(&cmd, &v, shell))
		return (NULL);
	return (finish_pars(v.cmds, shell, cmd));
}
