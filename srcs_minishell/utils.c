/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 20:35:33 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 20:49:29 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int is_redirection(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

// all these fucking functions need to be tested. to think about free and exit also
int	set_in_quotes_flag(char c, int *in_quotes, char **ptr)
{
	if (c == '\'' && *in_quotes != 2)
	{
		if (*in_quotes == 1)
			*in_quotes = 0;
		else
			*in_quotes = 1;
		(*ptr)++;
	}
	else if (c == '"' && *in_quotes != 1)
	{
		if (*in_quotes == 2)
			*in_quotes = 0;
		else
			*in_quotes = 2;
		(*ptr)++;
	}
	else
		return (0);
	return (1);
}

int	is_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}
