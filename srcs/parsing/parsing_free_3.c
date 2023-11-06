/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_free_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 23:06:42 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 23:07:07 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_get_var_name_and_err(t_cmd **struct_cmd, char **cmd,
					t_pars_vars *v)
{
	free_structs(struct_cmd);
	free_and_null(cmd);
	free(v->cmd_to_exec);
	free_all_envs(&v->shell->env);
	free_all_envs(&v->shell->exported_vars);
	malloc_err();
}

void	free_extract_cmd(t_cmd **cmds, char **str_cmd, t_shell *shell)
{
	free_structs(cmds);
	free(*str_cmd);
	free_all_envs(&shell->env);
	free_all_envs(&shell->exported_vars);
}
