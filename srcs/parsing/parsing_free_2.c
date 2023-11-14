/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_free_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 23:04:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 23:05:01 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_extract_cmd_2(t_cmd **cmds, t_pars_vars *v)
{
	free_structs(cmds);
	free_dbl_ptr(v->tokens);
}

void	free_extract_cmd_3(char **str_cmd, t_shell *shell)
{
	free(*str_cmd);
	free_all_envs(&shell->env);
	free_all_envs(&shell->exported_vars);
}

void	free_ext_cmd4_and_err(t_cmd **cmds, t_smplcmd *smplcmd,
					char **str, t_shell *shell)
{
	free_structs(cmds);
	free_smplcmd(smplcmd);
	free(*str);
	free_all_envs(&shell->env);
	free_all_envs(&shell->exported_vars);
	malloc_err();
}

void	free_if_question(char *key, char *var_value)
{
	if (ft_strcmp(key, "?") == 0)
		free(var_value);
	free(key);
}

void	free_replace_vars(t_cmd **cmds, char **str_cmd, t_pars_vars *v)
{
	free_structs(cmds);
	free_and_null(str_cmd);
	free_dbl_ptr(v->tokens);
	free(v->cmd_to_exec);
	free_all_envs(&v->shell->env);
	free_all_envs(&v->shell->exported_vars);
}
