/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:52:38 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/27 11:52:38 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_home_path(t_shell *shell)
{
	t_vars	*home_env;

	home_env = find_key("HOME", shell->env);
	if (!home_env)
		return (NULL);
	return (home_env->value);
}

static int	validate_and_set_path(char **path, t_shell *shell, t_smplcmd cmd)
{
	if (get_array_size(cmd.args) > 2)
		return (ft_putendl_fd("cd: too many arguments", 2), (EXIT_FAILURE));
	if (get_array_size(cmd.args) == 1)
	{
		(*path) = get_home_path(shell);
		if (!(*path))
			return (ft_putendl_fd("cd: HOME not set", 2), (EXIT_FAILURE));
	}
	else
		(*path) = cmd.args[1];
	if (access(*path, X_OK) != 0)
		return (ft_putstr_fd("cd: ", 2), perror(*path), (EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

int	execute_cd(t_smplcmd command, t_shell *shell)
{
	int		error_code;
	char	*path;
	char	*old_pwd_path;
	char	*pwd_path;

	if (validate_and_set_path(&path, shell, command) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	error_code = get_pwd_string(&old_pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (error_code);
	if (chdir(path) != 0)
		return (perror("cd"), free(old_pwd_path), (ERROR_FATAL));
	error_code = get_pwd_string(&pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (free(old_pwd_path), error_code);
	error_code = edit_shell_env(shell, pwd_path, old_pwd_path);
	return (error_code);
}
