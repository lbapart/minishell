/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:34:04 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/27 11:34:04 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_not_found(void)
{
	// TODO: Try getcwd(...)? Or just throw error message?
	return (EXIT_FAILURE);
}

	// char	path[4096];
	// int		error_code;

	// if (getcwd(path, sizeof(path)))
	// 	printf("%s\n", path);
	// else
	// {
	// 	error_code = errno;
	// 	return (perror("Error while executing getcwd"), (error_code)); //TODO: Handle error in here?
	// }
	// return (EXIT_SUCCESS);
int	execute_pwd(t_smplcmd command, t_shell shell)
{
	int	i;

	if (get_array_size(command.args) != 1)
	{
		ft_putstr_fd("Invalid args!\n", 2);
		return (EXIT_FAILURE);
	}
	i = 0;
	if (!shell.env)
		return (handle_pwd_not_found());
	while (shell.env[i])
	{
		if (ft_strncmp("PWD=", shell.env[i], 4) == 0)
		{
			ft_putendl_fd(shell.env[i] + 4, 1);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	return (handle_pwd_not_found());
}
