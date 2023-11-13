/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:30:21 by aapenko           #+#    #+#             */
/*   Updated: 2023/11/13 18:31:22 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_before(char *cmd, size_t n)
{
	while (n > 0)
	{
		if (is_redirection(cmd[n]))
			return (1);
		if (is_whitespace(cmd[n]))
			n--;
		else
			return (0);
	}
	if (is_redirection(cmd[n]))
		return (1);
	return (0);
}

static char	*check_quote_var(char *var, t_cmd **struct_cmd,
					char **cmd, t_pars_vars *v)
{
	char	*res;

	if (var[0] == '\'' || var[0] == '\"')
	{
		res = ft_strdup("");
		if (!res)
			return (free_get_var_name_and_err(struct_cmd, cmd, v), NULL);
		return (res);
	}
	return (NULL);
}

char	*get_var_name(char *var, t_cmd **struct_cmd, char **cmd, t_pars_vars *v)
{
	size_t	i;
	char	*res;

	i = 0;
	if (var[i] == '?')
	{
		res = ft_strdup("?");
		if (!res)
			return (free_get_var_name_and_err(struct_cmd, cmd, v), NULL);
		return (res);
	}
	while (var[i] && is_var_char(var[i]))
		i++;
	if (i == 0)
		return (check_quote_var(var, struct_cmd, cmd, v));
	res = (char *)malloc(i + 1);
	if (!res)
		return (free_get_var_name_and_err(struct_cmd, cmd, v), NULL);
	return (ft_strncpy(res, var, i), res);
}
