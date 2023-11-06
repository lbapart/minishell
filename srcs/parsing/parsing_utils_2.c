/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:26:08 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/06 22:29:06 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_until_pipe(char **str_cmd, char *cmd_to_exec,
					size_t last_pipe, size_t n)
{
	size_t	i;

	i = 0;
	while ((*str_cmd) && (*str_cmd)[i + last_pipe] && i + last_pipe < n)
	{
		cmd_to_exec[i] = (*str_cmd)[i + last_pipe];
		i++;
	}
	cmd_to_exec[i] = '\0';
}

void	set_hidden_quotes(char *cmd, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (cmd[i] == '\'')
			cmd[i] = HIDDEN_QUOTE;
		i++;
	}
}
