/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:11:12 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/15 18:20:45 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error_numeric_arg(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static int	is_valid_number(char *str, int is_negative)
{
	int		i;
	char	*max_value;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (print_error_numeric_arg(str), 0);
	}
	max_value = "9223372036854775807";
	if (is_negative)
		max_value = "9223372036854775808";
	if (ft_strlen(str) > 19
		|| (ft_strlen(str) == 19 && ft_strncmp(str, max_value, 20) > 0))
	{
		return (print_error_numeric_arg(str), 0);
	}
	return (1);
}

static int	exit_code_parse(char *str)
{
	long long	num;
	int			sign;
	int			i;

	if (str[0] == '\0')
		return (ft_putendl_fd("exit: : numeric argument required", 2), -1);
	i = 0;
	num = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!is_valid_number(str + i, sign == -1))
		return (-1);
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return ((unsigned char)(num * sign));
}

int	execute_exit(t_smplcmd command, t_shell *shell)
{
	int	exit_code;
	int	arg_amount;

	arg_amount = get_array_size(command.args);
	shell->is_exit = 1;
	if (arg_amount == 1)
		return (shell->last_exit_code);
	exit_code = exit_code_parse(command.args[1]);
	if (exit_code == -1)
		return (2);
	if (get_array_size(command.args) > 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		shell->is_exit = 0;
		return (EXIT_FAILURE);
	}
	return (exit_code);
}
