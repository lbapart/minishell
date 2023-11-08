/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:58:21 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/08 16:30:31 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *key, t_pars_vars *v, t_cmd **cmds, char **str_cmd)
{
	t_vars	*temp;
	char	*res;

	if (ft_strcmp(key, "?") == 0)
	{
		res = ft_itoa(v->shell->last_exit_code);
		if (!res)
			return (free(key), free_structs(cmds), free_and_null(str_cmd),
				free_all_envs(&v->shell->env),
				free_all_envs(&v->shell->exported_vars),
				free(v->cmd_to_exec), malloc_err(), NULL);
		return (res);
	}
	temp = v->shell->env;
	temp = find_key(key, temp);
	if (temp)
		return (temp->value);
	temp = v->shell->exported_vars;
	temp = find_key(key, temp);
	if (temp)
		return (temp->value);
	return (NULL);
}

void	replace_vars_with_values(char **str_cmd, t_pars_vars *v,
					t_cmd **cmds, t_shell *shell)
{
	t_pars_vars	i;
	char		*var_value;

	init_vars(&i, NULL, shell);
	while (v->cmd_to_exec[i.i])
	{
		if (v->cmd_to_exec[i.i] == '\'' && !i.is_open_single_quote)
			i.is_open_single_quote = !i.is_open_single_quote;
		else if (v->cmd_to_exec[i.i] == '\'' && i.is_open_single_quote)
			i.is_open_single_quote = !i.is_open_single_quote;
		if (v->cmd_to_exec[i.i] == '$' && !i.is_open_single_quote)
		{
			i.temp = get_var_name(v->cmd_to_exec + i.i + 1, cmds, str_cmd, v);
			if (!i.temp && ++i.i)
				continue ;
			var_value = get_var_value(i.temp, v, cmds, str_cmd);
			i.res = (char *)malloc(ft_strlen(v->cmd_to_exec)
					- (ft_strlen(i.temp) + 1) + ft_strlen(var_value) + 2);
			if (!i.res)
				return (free_if_question(i.temp, var_value),
					free_replace_vars(cmds, str_cmd, v), malloc_err());
			create_result_command(v, &i, var_value);
		}
		i.i++;
	}
}

t_cmd	*replace_all_vars_redir(t_cmd *cmd, t_shell *shell, char *strcmd)
{
	t_cmd	*temp;

	temp = cmd;
	while (temp)
	{
		if (temp->smplcmd->redir)
		{
			if (!replace_vars_in_redir(temp->smplcmd->redir,
					shell, cmd, strcmd))
				return (NULL);
		}
		temp = temp->next;
	}
	return (cmd);
}
