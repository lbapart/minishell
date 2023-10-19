/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:24:05 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:16:27 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->name)
		free(cmd->name);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

t_command	*parse_command(char *str, char **paths)
{
	int			i;
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (perror("Memory allocation failed"), (NULL));
	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	if (i == 0)
		return (free(cmd), (NULL));
	cmd->name = ft_substr(str, 0, i);
	if (!cmd->name)
		return (perror("Memory allocation failed"), free(cmd), NULL);
	cmd->args = ft_split(str, ' ');
	if (!cmd->args)
		return (perror("Memory allocation failed"), free_command(cmd), NULL);
	cmd->path = get_command_path(paths, cmd->name);
	if (!cmd->path)
		return (free_command(cmd), (NULL));
	return (cmd);
}
