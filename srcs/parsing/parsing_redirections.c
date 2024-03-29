/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:14:19 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/13 18:23:32 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_token(char *token)
{
	if (ft_strcmp(token, "<") == 0)
		return (1);
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, ">>") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

int	is_valid_token_for_redir(char *token)
{
	if (!token)
		return (0);
	if (token[0] == '\0')
		return (0);
	if (is_redir_token(token))
		return (0);
	return (1);
}

int	check_redir_tokens(char **tokens)
{
	size_t	i;
	int		result;

	result = 0;
	i = 0;
	while (tokens && tokens[i])
	{
		if (is_redir_token(tokens[i]))
		{
			if (result)
				return (0);
			result = 1;
		}
		else if (result && !is_valid_token_for_redir(tokens[i]))
			return (0);
		else
			result = 0;
		i++;
	}
	if (result)
		return (0);
	return (1);
}
