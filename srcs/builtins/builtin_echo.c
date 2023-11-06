/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 07:19:39 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/30 07:19:39 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(char **args, int arg_count, int isSkipNewLineEnd)
{
	int	i;

	i = 1 + isSkipNewLineEnd;
	while (i < arg_count - 1)
	{
		ft_putstr_fd(args[i], 1);
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (args[i])
		ft_putstr_fd(args[i], 1);
	if (!isSkipNewLineEnd)
		ft_putchar_fd('\n', 1);
}

int	execute_echo(t_smplcmd command)
{
	int	arg_count;

	arg_count = get_array_size(command.args);
	if (arg_count == 1)
	{
		ft_putstr_fd("\n", 1);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(command.args[1], "-n", 3) == 0)
	{
		print_args(command.args, arg_count, 1);
		return (EXIT_SUCCESS);
	}
	print_args(command.args, arg_count, 0);
	return (EXIT_SUCCESS);
}
