/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapenko <aapenko@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:25:46 by aapenko           #+#    #+#             */
/*   Updated: 2023/11/14 19:16:35 by aapenko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_clownshell(t_shell *shell, char **envp)
{
	shell->std_stdout = dup(STDOUT_FILENO);
	if (shell->std_stdout == -1)
		return (perror("dup"), EXIT_FAILURE);
	shell->std_stdin = dup(STDIN_FILENO);
	if (shell->std_stdin == -1)
		return (perror("dup"), close(shell->std_stdout), EXIT_FAILURE);
	if (init_env(envp, shell) != EXIT_SUCCESS)
		return (close(shell->std_stdout), close(shell->std_stdin),
			EXIT_FAILURE);
	shell->last_exit_code = 0;
	shell->exported_vars = NULL;
	shell->is_exit = 0;
	return (EXIT_SUCCESS);
}

int	main_while(char *line, t_shell *shell)
{
	if (line && line[0] != '\0')
		add_history(line);
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_code = 130;
		g_signal_received = 0;
	}
	exec_commands(line, shell);
	if (g_signal_received == SIGINT)
		shell->last_exit_code = 130;
	free(line);
	if (dup2(shell->std_stdout, STDOUT_FILENO) == -1)
		return (perror("dup2"), close_clownshell(shell, 0),
			exit(EXIT_FAILURE), (1));
	if (dup2(shell->std_stdin, STDIN_FILENO) == -1)
		return (perror("dup2"), close_clownshell(shell, 0),
			exit(EXIT_FAILURE), (1));
	return (EXIT_SUCCESS);
}

void	close_clownshell(t_shell *shell, int print_exit)
{
	if (print_exit)
		printf("exit\n");
	rl_clear_history();
	close(shell->std_stdin);
	close(shell->std_stdout);
	free_all_envs(&(shell->exported_vars));
	free_all_envs(&(shell->env));
}
