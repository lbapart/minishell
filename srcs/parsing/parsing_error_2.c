/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 23:01:33 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/08 13:44:55 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unexpected_near_pipe_err(void)
{
	ft_putstr_fd("🤡🤡🤡syntax error near unexpected token '|'🤡🤡🤡\n", 2);
}

void	amb_redir_err(void)
{
	ft_putstr_fd("🤡🤡🤡ambiguous redirect🤡🤡🤡\n", 2);
}
