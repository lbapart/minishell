/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:21:46 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/27 15:21:46 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_env(t_smplcmd command, t_shell shell)
{
	t_vars	*env;

	if (get_array_size(command.args) != 1)
	{
		ft_putstr_fd("Invalid args!", 2);
		return (EXIT_FAILURE);
	}
	// TODO: Check if env is not set env command EXIT_STATUS is still SUCCESS
	env = shell.env;
	if (!env)
		return (EXIT_SUCCESS);
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
