/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:02:11 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/07 17:51:29 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_filename(int pid)
{
	char	*filename;
	char	*tmp;

	filename = ft_itoa(pid);
	if (!filename)
		return (NULL);
	tmp = ft_strjoin(".tmp", filename);
	if (!tmp)
		return (free(filename), NULL);
	free(filename);
	filename = tmp;
	return (filename);
}

void	replace_redir_filename(t_redirection *redir, char *filename)
{
	free(redir->file);
	redir->file = filename;
}

//  1 = success, 0 = error, -1 = malloc error
int		exec_heredoc(t_redirection *redir, int pid)
{
	int		fd;
	char	*line;
	char	*filename;

	filename = generate_filename(pid);
    if (!filename)
        return (-1); // malloc error
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(filename), 0);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	if (close(fd) == -1)
		return (perror("close"), 0);
	replace_redir_filename(redir, filename);
	return (1);
}
