/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_waiting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:27:55 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/15 15:33:14 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_all_commands_on_error(t_cmd *start, t_cmd *cmd)
{
	int		status;

	status = EXIT_FAILURE;
	while (start != cmd)
	{
		waitpid(start->pid, &status, 0);
		start = start->next;
	}
	return (EXIT_FAILURE);
}

static int	handle_child_signals(int exit_code)
{
	if (exit_code == 2)
	{
		write(1, "\n", 1);
		exit_code = 130;
	}
	if (exit_code == 131)
	{
		write(2, "Quit (core dumped)\n", 19);
		exit_code = 131;
	}
	return (exit_code);
}

static int	wait_get_last_exit_code(t_cmd *cmd, int *is_error)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	while (cmd)
	{
		if (waitpid(cmd->pid, &exit_code, 0) == -1)
		{
			perror("waitpid");
			*is_error = 1;
		}
		if (WIFSIGNALED(exit_code) == 1)
		{
			if (exit_code == 2)
				exit_code = handle_child_signals(exit_code);
			else if (exit_code == 131)
				exit_code = handle_child_signals(exit_code);
		}
		else
			exit_code = WEXITSTATUS(exit_code);
		cmd = cmd->next;
	}
	return (exit_code);
}

int	handle_waiting_processes(t_cmd *cmd, t_shell *shell)
{
	int		exit_code;
	int		is_error;

	is_error = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exit_code = wait_get_last_exit_code(cmd, &is_error);
	if (is_error)
		return (EXIT_FAILURE);
	shell->last_exit_code = exit_code;
	return (EXIT_SUCCESS);
}
