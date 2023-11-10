/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:01:32 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 10:47:40 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_err(void)
{
	ft_putstr_fd("🤡🤡🤡Malloc failed🤡🤡🤡\n", 2);
	exit(MALLOCEXIT);
}

void	char_err(char c, t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡Unsupported character: ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("🤡🤡🤡\n", 2);
	shell->last_exit_code = 2;
}

void	unclosed_quotes_err(t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡Unclosed quotes error🤡🤡🤡\n", 2);
	shell->last_exit_code = 2;
}

void	double_pipe_err(t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡Double pipe error🤡🤡🤡\n", 2);
	shell->last_exit_code = 2;
}

void	redir_token_err(t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡Redirection error🤡🤡🤡\n", 2);
	shell->last_exit_code = 2;
}
