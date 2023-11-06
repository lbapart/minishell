/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 10:31:08 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/04 11:14:23 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_unset(t_smplcmd command, t_shell *shell)
{
	int	i;

	if (get_array_size(command.args) == 1)
		return (EXIT_SUCCESS);
	i = 1;
	while (command.args[i])
	{
		if (find_key(command.args[i], shell->env))
			remove_env_by_key(&(shell->env), command.args[i]);
		else if (find_key(command.args[i], shell->exported_vars))
			remove_env_by_key(&(shell->exported_vars), command.args[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
