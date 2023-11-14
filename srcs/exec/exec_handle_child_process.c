/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_child_process.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:40:46 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 17:02:22 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_and_clean_up(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	if (cmd->smplcmd->builtin == 0)
		exit_code = exec_simple_command(cmd->smplcmd, shell);
	else
		exit_code = exec_builtin(cmd->smplcmd, shell);
	free_child_process(cmd, shell);
	exit(exit_code);
}

void	close_pipe_free_process_exit(t_cmd *cmd, t_shell *shell)
{
	close_pipe(cmd);
	free_child_process(cmd, shell);
	exit(EXIT_FAILURE);
}

void	close_unused_fds(t_cmd *cmd, t_shell *shell)
{
	if (close(shell->std_stdin) == -1)
	{
		perror("close");
		close(shell->std_stdout);
		return (close_pipe_free_process_exit(cmd, shell));
	}
	if (close(shell->std_stdout) == -1)
		return (perror("close"), close_pipe_free_process_exit(cmd, shell));
}

static void	overwrite_redirections(t_cmd *cmd, t_shell *shell)
{
	int	exit_redirections;

	exit_redirections = handle_redirections(cmd->smplcmd->redir);
	if (exit_redirections != EXIT_SUCCESS)
		return (free_child_process(cmd, shell), exit(exit_redirections));
}

void	handle_child_process(t_cmd *cmd, t_shell *shell)
{
	close_unused_fds(cmd, shell);
	redirect_pipe_input(cmd, shell);
	redirect_pipe_output(cmd, shell);
	overwrite_redirections(cmd, shell);
	execute_and_clean_up(cmd, shell);
}
