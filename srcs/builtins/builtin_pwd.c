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

/*
if (!shell.env)
		return (handle_pwd_not_found());
	pwd = find_key("PWD", shell.env);
	if (!pwd)
		return (handle_pwd_not_found(), EXIT_FAILURE);
	ft_putendl_fd(pwd->value, 1);
	return (EXIT_SUCCESS);
*/

int	get_pwd_string(char **pwd)
{
	char	path[4096];
	int		error_code;

	if (getcwd(path, sizeof(path)))
	{
		*pwd = ft_strdup(path);
		if (!(*pwd))
			return (perror("Allocation Error"), (ERROR_FATAL));
		return (EXIT_SUCCESS);
	}
	else
	{
		error_code = errno;
		return (perror("Error while executing getcwd"), (error_code));
	}
}

int	execute_pwd(t_smplcmd command)
{
	char	*pwd_str;
	int		error_code;

	//TODO: Check for any additional args allowed?
	if (get_array_size(command.args) != 1)
	{
		ft_putstr_fd("Invalid args!\n", 2);
		return (ERROR_INVALID_ARGS);
	}
	error_code = get_pwd_string(&pwd_str);
	if (error_code != EXIT_SUCCESS)
		return (error_code);
	printf("%s\n", pwd_str);
	free(pwd_str);
	return (EXIT_SUCCESS);
}
