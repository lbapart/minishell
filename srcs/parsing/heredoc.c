/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:02:11 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/13 18:00:33 by aapenko          ###   ########.fr       */
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
	//TODO: join with shlvl 
	tmp = ft_not_free_strjoin(".tmp", filename);
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

	res = (char *)malloc(ft_strlen(str) - ft_strlen(var_name)
			+ ft_strlen(var_value) + 2);
	if (!res)
		return (free(str), NULL);
	ft_strncpy(res, str, pos);
	ft_strcat(res, var_value);
	ft_strcat(res, str + pos + ft_strlen(var_name) + 1);
	free(str);
	free(var_name);
	return (res);
}
