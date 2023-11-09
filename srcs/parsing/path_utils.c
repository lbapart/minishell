/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:27:29 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/09 12:30:26 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_vars *vars)
{
	t_vars	*temp;

	temp = vars;
	while (temp)
	{
		if (temp->value)
			printf("declare -x %s=\"%s\"\n", temp->key, temp->value);
		else
			printf("declare -x %s\n", temp->key);
		temp = temp->next;
	}
}

char	**get_paths(t_vars *env, t_cmd *cmd, t_shell *shell, char *strcmd)
{
	t_vars	*temp;
	char	**res;

	temp = env;
	while (temp)
	{
		if (ft_strncmp(temp->key, "PATH", 5) == 0)
		{
			res = ft_split(temp->value, ':');
			if (!res)
				return (free_structs(&cmd), free_all_envs(&shell->env),
					free_all_envs(&shell->exported_vars), free(strcmd),
					malloc_err(), NULL);
			return (res);
		}
		temp = temp->next;
	}
	return (NULL);
}

char	*get_command_path(char **paths, char *command_name)
{
	int		i;
	char	*path_str;
	char	*full_command;

	i = 0;
	if (access(command_name, X_OK) == 0)
		return (ft_strdup(command_name));
	while (paths[i])
	{
		path_str = ft_strjoin(paths[i], "/");
		if (!path_str)
			return (NULL);
		full_command = ft_strjoin(path_str, command_name);
		if (!full_command)
			return (NULL);
		free(path_str);
		if (access(full_command, X_OK) == 0)
			return (full_command);
		free(full_command);
		i++;
	}
	return (ft_strdup(command_name));
}

t_cmd	*replace_with_absolute_path(t_cmd *cmd, t_shell *shell, char *strcmd)
{
	t_cmd	*temp;
	char	**paths;
	char	*full_command;

	temp = cmd;
	paths = get_paths(shell->env, cmd, shell, strcmd);
	if (!paths)
		paths = get_paths(shell->exported_vars, cmd, shell, strcmd);
	if (!paths)
		return (cmd);
	while (temp)
	{
		if (temp->smplcmd->path && temp->smplcmd->builtin == 0)
		{
			full_command = get_command_path(paths, temp->smplcmd->path);
			if (!full_command)
				return (free_structs(&cmd), free_all_envs(&shell->env),
					free_all_envs(&shell->exported_vars), free(strcmd),
					free_dbl_ptr(paths), malloc_err(), NULL);
			free(temp->smplcmd->path);
			temp->smplcmd->path = full_command;
		}
		temp = temp->next;
	}
	return (free_dbl_ptr(paths), cmd);
}
