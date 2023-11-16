/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:21:32 by lbapart           #+#    #+#             */
/*   Updated: 2023/11/16 09:37:05 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

int	g_signal_received = 0;

static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	g_signal_received = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void	main_when_child_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
}

void	init_signals(int mode)
{
	if (mode == GLOBAL_MODE)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, SIG_IGN);
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
	if (mode == MAIN_WHEN_CHILD_MODE)
	{
		signal(SIGINT, &main_when_child_handler);
		signal(SIGQUIT, SIG_DFL);
	}
}
