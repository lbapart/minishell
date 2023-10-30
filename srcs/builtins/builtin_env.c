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
	int	i;

	if (get_array_size(command.args) != 1)
	{
		ft_putstr_fd("Invalid args!", 2);
		return (EXIT_FAILURE);
	}
	i = 0;
	// TODO: Check if env is not set env command EXIT_STATUS is still SUCCESS
	if (!shell.env)
		return (EXIT_SUCCESS);
	while (shell.env[i])
	{
		ft_putendl_fd(shell.env[i], 1);
		i++;
	}
	return (EXIT_SUCCESS);
}
