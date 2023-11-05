/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:45:58 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/05 19:08:10 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	remove_unnecessary_quotes(char *str)
{
	char	*str_in;
	char	*str_out;
	int		in_single_quotes;
	int		in_double_quotes;

	if (!str)
		return ;
	str_in = str;
	str_out = str;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str_in)
	{
		if (*str_in == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			str_in++;
		}
		else if (*str_in == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			str_in++;
		}
		else
			*(str_out++) = *(str_in++);
	}
	*str_out = '\0';
}

int	check_unclosed_quotes(char *cmd)
{
	int		in_quotes;

	in_quotes = 0;
	while (*cmd)
	{
		if (*cmd == '\'' && in_quotes != 2)
		{
			if (in_quotes == 1)
				in_quotes = 0;
			else
				in_quotes = 1;
		}
		else if (*cmd == '"' && in_quotes != 1)
		{
			if (in_quotes == 2)
				in_quotes = 0;
			else
				in_quotes = 2;
		}
		cmd++;
	}
	if (in_quotes)
		return (0);
	return (1);
}

int	check_and_put_path(char **tokens, t_smplcmd *smplcmd)
{
	if (tokens && tokens[0] && (tokens[0][0] == '/' || tokens[0][0] == '.'))
	{
		smplcmd->path = ft_strdup(tokens[0]);
		if (!smplcmd->path)
			return (0);
	}
	else
	{
		smplcmd->path = NULL;
	}
	return (1);
}

char	*get_var_name(char *var, char **tokens, t_cmd *t_cmd, char **cmd)
{
	size_t	i;
	char	*res;

	i = 0;
	while (var[i] && is_var_char(var[i]))
		i++;
	if (i == 0)
		return (NULL);
	res = (char *)malloc(i + 1);
	if (!res)
		return (free_dbl_ptr(tokens), free_structs(t_cmd),
			free_and_null(cmd), exit(MALLOCEXIT), NULL);
	ft_strncpy(res, var, i);
	return (res);
}
