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

int	add_pwd_to_env(char *keystr, char *value, t_shell *shell)
{
	char	*key;
	t_vars	*pwd_env;
	
	key = ft_strdup(keystr);
	if (!key)
			return(perror("Allocation failed"), ERROR_FATAL);
	pwd_env = new_env(key, value);
	if (!pwd_env)
		return (ERROR_FATAL);
	add_env(&(shell->env), pwd_env);
	return (EXIT_SUCCESS);
}

int	edit_shell_env(t_shell *shell, char *pwd, char *old_pwd)
{
	t_vars	*pwd_env;
	t_vars	*old_pwd_env;

	pwd_env = find_key("PWD", shell->env);
	if (pwd_env)
		pwd_env->value = pwd;
	else
	{
		if (add_pwd_to_env("PWD", pwd, shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	old_pwd_env = find_key("OLDPWD", shell->env);
	if (old_pwd_env)
		old_pwd_env->value = old_pwd;
	else
	{
		if (add_pwd_to_env("OLDPWD", old_pwd, shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	execute_cd(t_smplcmd command, t_shell *shell)
{
	int	error_code = 0;
	char	*path;
	char	*old_pwd_path;
	char	*pwd_path;

	if (get_array_size(command.args) != 2)
		return (ft_putendl_fd("Invalid amount of args for cd!", 2), (ERROR_INVALID_ARGS));
	//TODO: Implement `cd` to change to $HOME
	path = command.args[1];
	if (access(path, X_OK) != 0)
	{
		error_code = errno;
		return (perror("Error while accessing directory!"), (error_code)); 
	}
	error_code = get_pwd_string(&old_pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (error_code);
	if (chdir(path) != 0)
		return (perror("Error while changing directory!"), free(old_pwd_path), (ERROR_FATAL));
	error_code = get_pwd_string(&pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (free(old_pwd_path), error_code);
	edit_shell_env(shell, pwd_path, old_pwd_path);
	return (error_code);
}
