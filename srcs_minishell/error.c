/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:01:32 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/30 19:07:03 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	malloc_err(void)
{
	ft_putstr_fd("🤡🤡🤡Malloc failed🤡🤡🤡\n", 2);
	exit(MALLOCEXIT);
}

void	unsupported_char_err(char c)
{
	ft_putstr_fd("🤡🤡🤡Unsupported character: ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("🤡🤡🤡\n", 2);
}

void	unclosed_quotes_err(void)
{
	ft_putstr_fd("🤡🤡🤡Unclosed quotes error🤡🤡🤡\n", 2);
}

void	double_pipe_err(void)
{
	ft_putstr_fd("🤡🤡🤡Double pipe error🤡🤡🤡\n", 2);
}
