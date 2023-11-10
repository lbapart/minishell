/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:02:11 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 12:35:01 by ppfiel           ###   ########.fr       */
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
int	exec_heredoc(t_redirection *redir, int pid)
{
	int		fd;
	char	*line;
	char	*filename;

	filename = generate_filename(pid);
	if (!filename)
		return (EXIT_FAILURE);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(filename), EXIT_FAILURE);
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
		(write(fd, line, ft_strlen(line)), write(fd, "\n", 1), free(line));
	}
	if (close(fd) == -1)
		return (perror("close"), EXIT_FAILURE);
	return (replace_redir_filename(redir, filename), EXIT_SUCCESS);
}
