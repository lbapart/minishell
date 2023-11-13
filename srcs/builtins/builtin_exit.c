/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:11:12 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/06 07:56:29 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_number(char *str, int is_negative)
{
	int		i;
	char	*max_value;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd(": numeric argument required", 2);
			return (0);
		}
	}
	max_value = "9223372036854775807";
	if (is_negative)
		max_value = "9223372036854775808";
	if (ft_strlen(str) > 19 || (ft_strlen(str) == 19 && ft_strncmp(str, max_value, 20) > 0))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (0);
	}
	return (1);
}

int	exit_code_parse(char *str, t_shell *shell)
{
	long long	num;
	int			sign;
	int			i;

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
		return (2);
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	shell->is_exit = 1;
	return ((unsigned char)(num * sign));
}

int	execute_exit(t_smplcmd command, t_shell *shell)
{
	if (get_array_size(command.args) > 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (get_array_size(command.args) == 1)
	{
		shell->is_exit = 1;
		return (EXIT_SUCCESS);
	}
	return (exit_code_parse(command.args[1], shell));
}
