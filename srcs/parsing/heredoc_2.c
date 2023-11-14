/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:00:39 by aapenko           #+#    #+#             */
/*   Updated: 2023/11/14 18:53:27 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_key_exists(char *var_name, t_shell *shell)
{
	t_vars	*temp;

	if (ft_strcmp(var_name, "?") == 0)
		return (1);
	temp = shell->env;
	temp = find_key(var_name, temp);
	if (temp)
		return (1);
	temp = shell->exported_vars;
	temp = find_key(var_name, temp);
	if (temp)
		return (1);
	return (0);
}

int	check_varname(char *var_name, size_t *i, t_shell *shell)
{
	(void)shell;
	if (!var_name[0])
	{
		(*i)++;
		free(var_name);
		return (1);
	}
	return (0);
}

char	*replace_vars_heredoc(char *str, t_shell *shell, char *var_value)
{
	size_t	i;
	char	*var_name;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			var_name = get_var_name_heredoc(str + i + 1);
			if (!var_name)
				return (free(str), NULL);
			if (check_varname(var_name, &i, shell))
				continue ;
			var_value = get_var_value_heredoc(var_name, shell);
			if (!var_value)
				return (free(var_name), free(str), NULL);
			str = replace_var_with_value_heredoc(str, var_value, var_name, i);
			if (!str)
				return (free(var_name), free(var_value), NULL);
			i += ft_strlen(var_value) - 1;
			free(var_value);
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
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line == NULL)
				break ;
			line = ft_strtrim(line, "\n");
		}
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file", 2);
			ft_putstr_fd(" (wanted `", 2);
			ft_putstr_fd(eof, 2);
			ft_putendl_fd("')", 2);
			return (EXIT_FAILURE);
		}
		if (g_signal_received == SIGINT)
			return (EXIT_FAILURE);
		if (ft_strcmp(line, eof) == 0)
			return (free(line), EXIT_SUCCESS);
		result_line = replace_vars_heredoc(line, shell, NULL);
		if (!result_line)
			return (EXIT_FAILURE);
		ft_putendl_fd(result_line, fd);
		free(result_line);
	}
	return (1);
}

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
		return (free(eof), perror(filename), free(filename), EXIT_FAILURE);
	replace_redir_filename(redir, filename);
	redir->to_delete = 1;
	if (read_and_put_in_file(fd, eof, shell) == EXIT_FAILURE)
		return (free(eof), close(fd), EXIT_FAILURE);
	if (close(fd) == -1)
		return (free(eof), perror("close"), EXIT_FAILURE);
	return (free(eof), EXIT_SUCCESS);
}
