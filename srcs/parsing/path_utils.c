/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:27:29 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:01:28 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_not_free_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	size_t	i;
	size_t	j;
	char	*str;

	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = malloc(size);
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		str[j + i] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
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
	if (command_name && (command_name[0] == '.' || command_name[0] == '/'))
		return (ft_strdup(command_name));
	if (access(command_name, X_OK) == 0)
		return (ft_strdup(command_name));
	while (paths[i])
	{
		path_str = ft_not_free_strjoin(paths[i], "/");
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
