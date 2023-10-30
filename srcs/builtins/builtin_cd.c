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

int	get_array_size(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}

int	get_env_size(char **env, t_shell *shell, int *is_pwd_set, int *is_old_pwd_set)
{
	int	size;

	size = 0;
	*is_pwd_set = 0;
	*is_old_pwd_set = 0;
	while (env[size])
	{
		if (ft_strncmp("PWD=", shell->env[size], 4) == 0)
			*is_pwd_set = 1;
		else if (ft_strncmp("OLDPWD=", shell->env[size], 7) == 0)
			*is_old_pwd_set = 1;
		size++;
	}
	return (size);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	join_path_env(char *path, char *join_value, char **env, int i)
{
	int	error_code;

	env[i] = ft_strjoin("PWD=", path);
	if (!env[i])
	{
		error_code = errno;
		free_env(env);
		perror("Allocation failed");
		return (error_code);
	}
	return (EXIT_SUCCESS);
}

int	edit_shell_env(t_shell *shell, char *pwd, char *old_pwd)
{
	int	i;
	int	is_pwd_set;
	int	is_old_pwd_set;
	char **new_env; 	//TODO: malloc new env (check if PWD and OLDPWD is contained, otherwise add them)
	int	error_code;

	new_env = ft_calloc(get_env_size(shell->env, shell, &is_pwd_set, &is_old_pwd_set) + 1 + is_pwd_set + is_old_pwd_set, sizeof(char *));
	if (!new_env)
	{
		error_code = errno;
		return (perror("Allocation failed!"), (error_code));
	}
	i = 0;
	while(shell->env[i])
	{
		if (ft_strncmp("PWD=", shell->env[i], 4) == 0)
		{
			error_code = join_path_env(pwd, "PWD=", new_env, i);
			if (error_code != EXIT_SUCCESS)
				return (error_code);
		}
		else if (ft_strncmp("OLDPWD=", shell->env[i], 7) == 0)
		{
			error_code = join_path_env(old_pwd, "OLDPWD=", new_env, i);
			if (error_code != EXIT_SUCCESS)
				return (error_code);
		}
		else
		{
			new_env[i] = ft_strdup(shell->env[i]);
			if (!new_env[i])
			{
				error_code = errno;
				return (free_env(new_env), perror("Allocation failed"), error_code);
			}
		}
		i++;
	}
	if (!is_pwd_set)
	{
		error_code = join_path_env(pwd, "PWD=", new_env, i);
		if (error_code != EXIT_SUCCESS)
			return (error_code);
		i++;
	}
	if (!is_old_pwd_set)
	{
		error_code = join_path_env(old_pwd, "OLDPWD=", new_env, i);
		if (error_code != EXIT_SUCCESS)
			return (error_code);
		i++;
	}
	new_env[i] = NULL;
	free_env(shell->env);
	shell->env = new_env;
	return (EXIT_SUCCESS);
}


int	get_pwd_string(char **pwd)
{
	char	path[4096];
	int		error_code;

	if (getcwd(path, sizeof(path)))
	{
		*pwd = ft_strdup(path);
		if (!(*pwd))
		{
			error_code = errno;
			return (perror("Allocation Error"), (error_code));
		}
		return (EXIT_SUCCESS);
	}
	else
	{
		error_code = errno;
		return (perror("Error while executing getcwd"), (error_code));
	}
}

int	execute_cd(t_smplcmd command, t_shell *shell)
{
	int	error_code = 0;
	char	*path;
	char	*old_pwd_path;
	char	*pwd_path;

	if (get_array_size(command.args) != 2)
		return (ft_putendl_fd("Invalid amount of args for cd!", 2), (EXIT_FAILURE)); //TODO: Valid args check in here?
	path = command.args[1];
	if (access(path, X_OK) != 0)
	{
		error_code = errno;
		return (perror("Error while accessing directory!"), (error_code)); 
	}
	error_code = get_pwd_string(&old_pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (error_code);
	printf("OLD-PWD:%s\n", old_pwd_path);
	if (chdir(path) != 0)
	{
		error_code = errno;
		return (perror("Error while changing directory!"), (error_code));
	}
	error_code = get_pwd_string(&pwd_path);
	if (error_code != EXIT_SUCCESS)
		return (error_code); //TODO: Do we have to undo cd, when we want to continue in loop prompt ? Or is this a fatal Error?
	printf("NEW-PWD: %s\nOLD-PWD:%s\n", pwd_path, old_pwd_path);
	edit_shell_env(shell, pwd_path, old_pwd_path);
	free(old_pwd_path);
	free(pwd_path);
	return (error_code);
}