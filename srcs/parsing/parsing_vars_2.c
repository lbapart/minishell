/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_vars_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:29:34 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 10:37:10 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name_str(char *str, t_shell *shell, t_cmd *cmd, char *strcmd)
{
	size_t	i;
	char	*res;

	i = 0;
	if (str[i] == '?')
	{
		res = ft_strdup("?");
		if (!res)
			return (free_structs(&cmd), free_all_envs(&shell->env),
				free_all_envs(&shell->exported_vars), free(strcmd),
				malloc_err(), NULL);
		return (res);
	}
	while (str[i] && is_var_char(str[i]))
		i++;
	if (i == 0)
		return (NULL);
	res = (char *)malloc(i + 1);
	if (!res)
		return (free_structs(&cmd), free_all_envs(&shell->env), free(strcmd),
			free_all_envs(&shell->exported_vars), malloc_err(), NULL);
	ft_strncpy(res, str, i);
	return (res);
}

char	*get_var_value_str(char *var_name, t_shell *shell,
					t_cmd *cmd, char *strcmd)
{
	t_vars	*temp;
	char	*res;

	if (ft_strcmp(var_name, "?") == 0)
	{
		res = ft_itoa(shell->last_exit_code);
		if (!res)
			return (free_structs(&cmd), free_all_envs(&shell->env),
				free_all_envs(&shell->exported_vars), free(strcmd),
				malloc_err(), NULL);
		return (res);
	}
	temp = shell->env;
	temp = find_key(var_name, temp);
	if (temp)
		return (temp->value);
	temp = shell->exported_vars;
	temp = find_key(var_name, temp);
	if (temp)
		return (temp->value);
	return (NULL);
}

char	*replace_var_with_value(char **str, char *var_value,
					char *var_name, size_t pos)
{
	char	*res;

	res = (char *)malloc(ft_strlen(*str) - ft_strlen(var_name)
			+ ft_strlen(var_value) + 2);
	if (!res)
		return (free_and_null(str), free(var_name), NULL);
	ft_strncpy(res, *str, pos);
	ft_strcat(res, var_value);
	ft_strcat(res, *str + pos + ft_strlen(var_name) + 1);
	free_and_null(str);
	free(var_name);
	return (res);
}

char	*replace_vars_in_str(char **str, t_shell *shell,
					t_cmd *cmd, char *strcmd)
{
	size_t	i;
	char	*var_name;
	char	*var_value;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == REDIR_HIDDEN_DOLLAR)
		{
			(*str)[i] = '$';
			var_name = get_var_name_str(*str + i + 1, shell, cmd, strcmd);
			if (!var_name && ++i)
				continue ;
			var_value = get_var_value_str(var_name, shell, cmd, strcmd);
			(*str) = replace_var_with_value(str, var_value, var_name, i);
			if (!(*str))
				return (free_extract_cmd(&cmd, &strcmd, shell),
					malloc_err(), NULL);
			if (!(*str)[0])
				return (free_and_null(str), NULL);
			i += ft_strlen(var_value) - 1;
		}
		i++;
	}
	return (*str);
}

int	replace_vars_in_redir(t_redirection *redir, t_shell *shell,
					t_cmd *cmd, char *strcmd)
{
	t_redirection	*temp;

	temp = redir;
	while (temp)
	{
		if (temp->file)
		{
			temp->file = replace_vars_in_str(&temp->file, shell, cmd, strcmd);
			if (!temp->file || check_whitespace(temp->file))
				return (free_structs(&cmd), amb_redir_err(shell), 0);
		}
		temp = temp->next;
	}
	return (1);
}
