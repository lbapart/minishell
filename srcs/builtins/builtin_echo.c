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

	i = 0;
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

int	is_valid_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-' || ft_strlen(str) <= 1)
		return (0);
	while (str[++i])
	{
		if (str[i] != 'n')
			return (0);
	}
	return (1);
}

int	execute_echo(t_smplcmd command)
{
	int	arg_count;
	int	i;
	int	is_new_line_skip;

	arg_count = get_array_size(command.args);
	if (arg_count == 1)
	{
		ft_putstr_fd("\n", 1);
		return (EXIT_SUCCESS);
	}
	i = 1;
	is_new_line_skip = 0;
	while (command.args[i] && is_valid_flag(command.args[i]))
	{
		is_new_line_skip = 1;
		i++;
	}
	print_args(command.args + i, arg_count - i, is_new_line_skip);
	return (EXIT_SUCCESS);
}
