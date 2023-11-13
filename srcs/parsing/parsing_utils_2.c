/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:26:08 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/13 11:10:10 by ppfiel           ###   ########.fr       */
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

void	replace_dollar_sign(char *cmd, size_t *i,
				int *in_quotes, int dollar_type)
{
	while (is_whitespace(cmd[*i]))
		(*i)++;
	while (cmd[*i] && !(!*in_quotes && (is_whitespace(cmd[*i])
				|| is_redirection(cmd[*i]) || cmd[*i] == '|')))
	{
		if (cmd[*i] == '$'
			&& (cmd[*i + 1] == '\'' || cmd[*i + 1] == '\"'))
			;
		else if (cmd[*i] == '$')
			cmd[*i] = dollar_type;
		set_in_quotes_var(cmd[*i], in_quotes);
		(*i)++;
	}
}

int	check_unclosed_quotes(char *cmd)
{
	int		in_quotes;
	size_t	i;

	in_quotes = 0;
	i = 0;
	while (cmd[i])
	{
		set_in_quotes_var(cmd[i], &in_quotes);
		if (i > 0 && check_heredoc_before(cmd, i - 1) && in_quotes == 0)
			replace_dollar_sign(cmd, &i, &in_quotes, HEREDOC_HIDDEN_DOLLAR);
		else if (i > 0 && check_redirection_before(cmd, i - 1)
			&& in_quotes == 0)
			replace_dollar_sign(cmd, &i, &in_quotes, REDIR_HIDDEN_DOLLAR);
		if (!cmd[i])
			break ;
		i++;
	}
	if (in_quotes)
		return (0);
	return (1);
}

int	check_whitespace(char *str)
{
	size_t	i;
	int		in_quotes;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		set_in_quotes_var(str[i], &in_quotes);
		if (!in_quotes && is_whitespace(str[i]))
			return (1);
		i++;
	}
	return (0);
}
