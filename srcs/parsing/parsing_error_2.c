/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 23:01:33 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 10:44:15 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	near_pipe_err(t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡syntax error near unexpected token '|'🤡🤡🤡\n", 2);
	shell->last_exit_code = 2;
}

void	amb_redir_err(t_shell *shell)
{
	ft_putstr_fd("🤡🤡🤡ambiguous redirect🤡🤡🤡\n", 2);
	shell->last_exit_code = 1;
}
