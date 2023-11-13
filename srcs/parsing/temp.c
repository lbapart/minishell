/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:43:18 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/13 17:58:27 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//functions for testing
#include "minishell.h"

void	print_commands(t_cmd *cmds)
{
	t_cmd *tmp;
	t_smplcmd *smplcmd;
	t_redirection *temp;
	size_t i;

	tmp = cmds;
	while (tmp)
	{
		printf("--------------------\n");
		smplcmd = tmp->smplcmd;
		printf("path: %s\n", smplcmd->path);
		i = 0;
		while (smplcmd->args && smplcmd->args[i])
		{
			printf("arg %zu: %s\n", i, smplcmd->args[i]);
			i++;
		}
		temp = smplcmd->redir;
		while (temp)
		{
			printf("redir type: %d\n", temp->type);
			printf("redir file: %s\n", temp->file);
			temp = temp->next;
		}
		tmp = tmp->next;
		printf("--------------------\n");
	}
}

static char	*ft_temp_realloc(char *old_buf, char c)
{
	int		len;
	char	*res;
	int		i;

	len = ft_strlen(old_buf);
	res = (char *)malloc(len + 2);
	if (!res)
		return (free(old_buf), NULL);
	i = 0;
	while (old_buf[i])
	{
		res[i] = old_buf[i];
		i++;
	}
	res[i++] = c;
	res[i] = '\0';
	free(old_buf);
	return (res);
}

static char	*finish_get_next_line(char *buffer, int i, int read_bytes)
{
	buffer[i] = '\0';
	if (i == 0 || (!buffer[i - 1] && !read_bytes))
		return (free(buffer), NULL);
	return (buffer);
}

char	*get_next_line(int fd)
{
	int		read_bytes;
	char	c;
	char	*buffer;

	buffer = (char *)malloc(1);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	read_bytes = 1;
	while (read_bytes > 0)
	{
		read_bytes = read(fd, &c, 1);
		if (read_bytes == -1)
			return (free(buffer), NULL);
		else if (read_bytes == 0)
			break ;
		buffer = ft_temp_realloc(buffer, c);
		if (!buffer)
			return (NULL);
		if (c == '\n')
			break ;
	}
	return (finish_get_next_line(buffer, ft_strlen(buffer), read_bytes));
}