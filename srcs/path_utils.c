/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:27:29 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/28 20:32:15 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (ft_split(env[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

int	handle_name_full_path(char *command_name, char **full_command)
{
	if (access(command_name, X_OK) == 0)
	{
		*full_command = ft_strdup(command_name);
		if (!(*full_command))
			return (perror("Memory allocation failed"), (1));
		return (1);
	}
	return (0);
}

char	*get_command_path(char **paths, char *command_name)
{
	int		i;
	char	*path_str;
	char	*full_command;

	i = 0;
	if (handle_name_full_path(command_name, &full_command))
		return (full_command);
	while (paths[i])
	{
		path_str = ft_strjoin(paths[i], "/");
		if (!path_str)
			return (perror("Memory Allocation failed!"), (NULL));
		full_command = ft_strjoin(path_str, command_name);
		free(path_str);
		if (!full_command)
			return (perror("Memory Allocation failed!"), (NULL));
		if (access(full_command, X_OK) == 0)
			return (full_command);
		free(full_command);
		i++;
	}
	return (NULL);
}
