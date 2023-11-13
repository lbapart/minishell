/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:45:58 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/13 12:54:13 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_in_quotes(char *str)
{
	if (!ft_strcmp(str, "\">>\"") || !ft_strcmp(str, "\'>>\'"))
		return (1);
	if (!ft_strcmp(str, "\"<<\"") || !ft_strcmp(str, "\'<<\'"))
		return (1);
	if (!ft_strcmp(str, "\">\"") || !ft_strcmp(str, "\'>\'"))
		return (1);
	if (!ft_strcmp(str, "\"<\"") || !ft_strcmp(str, "\'<\'"))
		return (1);
	return (0);
}

void	replace_with_bullshit(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			str[i] = BULLSHIT;
		i++;
	}
}

void	remove_unnecessary_quotes(char *str)
{
	char	*str_in;
	char	*str_out;
	int		in_single_quotes;
	int		in_double_quotes;

	if (!str)
		return ;
	if (is_redir_in_quotes(str))
		return (replace_with_bullshit(str));
	str_in = str;
	str_out = str;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str_in)
	{
		if (*str_in == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*str_in == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			*(str_out++) = *(str_in);
		str_in++;
	}
	*str_out = '\0';
}

void	set_in_quotes_var(char c, int *in_quotes)
{
	if (c == '\'' && *in_quotes != 2)
	{
		if (*in_quotes == 1)
			*in_quotes = 0;
		else
			*in_quotes = 1;
	}
	else if (c == '"' && *in_quotes != 1)
	{
		if (*in_quotes == 2)
			*in_quotes = 0;
		else
			*in_quotes = 2;
	}
}

int	check_heredoc_before(char *cmd, size_t n)
{
	while (n > 0)
	{
		if (cmd[n] == '<' && cmd[n - 1] == '<')
			return (1);
		if (is_whitespace(cmd[n]))
			n--;
		else
			return (0);
	}
	return (0);
}

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

static char	*check_quote_var(char *var, t_cmd **struct_cmd, char **cmd, t_pars_vars *v)
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
