/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:02:11 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/11 12:37:09 by lbapart          ###   ########.fr       */
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


char	*get_var_name_heredoc(char *str)
{
	size_t	i;
	char	*res;

	i = 0;
	if (str[i] == '?')
	{
		res = ft_strdup("?");
		if (!res)
			return (NULL);
		return (res);
	}
	while (str[i] && is_var_char(str[i]))
		i++;
	if (i == 0)
		return (ft_strdup(""));
	res = (char *)malloc(i + 1);
	if (!res)
		return (NULL);
	ft_strncpy(res, str, i);
	return (res);
}

char	*get_var_value_heredoc(char *var_name, t_shell *shell)
{
	t_vars	*temp;
	char	*res;

	if (ft_strcmp(var_name, "?") == 0)
	{
		res = ft_itoa(shell->last_exit_code);
		if (!res)
			return (NULL);
		return (res);
	}
	temp = shell->env;
	temp = find_key(var_name, temp);
	if (temp)
		return (ft_strdup(temp->value));
	temp = shell->exported_vars;
	temp = find_key(var_name, temp);
	if (temp)
		return (ft_strdup(temp->value));
	return (ft_strdup(""));
}

char	*replace_var_with_value_heredoc(char *str, char *var_value,
					char *var_name, size_t pos)
{
	char	*res;

	res = (char *)malloc(ft_strlen(*str) - ft_strlen(var_name)
			+ ft_strlen(var_value) + 2);
	if (!res)
		return (free(str), NULL);
	ft_strncpy(res, *str, pos);
	ft_strcat(res, var_value);
	ft_strcat(res, *str + pos + ft_strlen(var_name) + 1);
	free(str);
	free(var_name);
	free(var_value);
	return (res);
}

char	*replace_vars_heredoc(char *str, t_shell *shell)
{
	size_t	i;
	char	*var_name;
	char	*var_value;

	i = 0;
	while (str[i])
	{
		if (str[i] == "$")
		{
			var_name = get_var_name_heredoc(str + i + 1);
			if (!var_name)
				return (NULL);
			if (!var_name[0] && ++i)
			{
				free(var_name);
				continue ;
			}
			var_value = get_var_value_heredoc(var_name, shell);
			if (!var_value)
				return (free(var_name), NULL);
			str = replace_var_with_value_heredoc(str, var_value, var_name, i);
			if (!str)
				return (free(var_name), free(var_value), NULL);
			i += ft_strlen(var_value) - 1;
		}
		i++;
	}
	return (str);
}

int	read_and_put_in_file(int fd, char *eof, t_shell *shell)
{
	char	*line;
	char	*result_line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (1);
		if (ft_strcmp(line, eof) == 0)
			return (free(line), 1);
		result_line = replace_vars_heredoc(line, shell);
		(write(fd, line, ft_strlen(line)), write(fd, "\n", 1), free(line));
	}
	return (1);
}

//  1 = success, 0 = error, -1 = malloc error
int	exec_heredoc(t_redirection *redir, int pid, t_shell *shell)
{
	int		fd;
	char	*filename;
	char	*eof;

	eof = ft_strdup(redir->file);
	if (!eof)
		return (EXIT_FAILURE);
	filename = generate_filename(pid);
	if (!filename)
		return (free(eof), EXIT_FAILURE);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(eof), perror(filename), EXIT_FAILURE);
	replace_redir_filename(redir, filename);
	// read_and_put_in_file(fd, eof, shell);
	if (close(fd) == -1)
		return (perror("close"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
