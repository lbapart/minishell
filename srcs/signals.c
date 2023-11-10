/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:21:32 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/10 14:26:30 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	signal_received = SIGQUIT;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	signal_received = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	init_signals(int mode)
{
	if (mode == GLOBAL_MODE)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, &sigquit_handler);
	}
	if (mode == HEREDOC_MODE)
	{
		signal(SIGINT, &heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	if (mode == CHILD_MODE)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}