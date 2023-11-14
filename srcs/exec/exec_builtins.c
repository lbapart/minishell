/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:34:37 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:34:54 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_smplcmd *smplcmd, t_shell *shell)
{
	if (smplcmd->builtin == ECHO)
		return (execute_echo(*smplcmd));
	if (smplcmd->builtin == CD)
		return (execute_cd(*smplcmd, shell));
	if (smplcmd->builtin == PWD)
		return (execute_pwd());
	if (smplcmd->builtin == EXPORT)
		return (execute_export(shell, *smplcmd));
	if (smplcmd->builtin == UNSET)
		return (execute_unset(*smplcmd, shell));
	if (smplcmd->builtin == ENV)
		return (execute_env(*smplcmd, *shell));
	if (smplcmd->builtin == EXIT)
		return (execute_exit(*smplcmd, shell));
	return (EXIT_FAILURE);
}
