/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_validating.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:12:50 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 10:23:10 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_key(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || ft_isdigit(str[0]) || str[0] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (0);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	handle_export(char *arg, t_shell *shell, int *exit_code)
{
	int	error_code;

	error_code = handle_variable_assignment(arg, shell);
	if (error_code == EXIT_FAILURE)
		*exit_code = EXIT_FAILURE;
	else if (error_code == -1)
	{
		if (handle_add_to_exported(arg, shell) != EXIT_SUCCESS)
			return (0);
	}
	return (1);
}
