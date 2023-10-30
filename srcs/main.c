/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 13:43:11 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:42:40 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	get_failure_and_perror(char *message)
// {
// 	perror(message);
// 	return (EXIT_FAILURE);
// }

// int	clean_up(t_pipex *pipex)
// {
// 	int	status;
// 	int	exit_status;

// 	exit_status = EXIT_SUCCESS;
// 	if (close(pipex->pipefd[0]) == -1)
// 		exit_status = get_failure_and_perror("Error while closing read-pipe");
// 	if (close(pipex->pipefd[1]) == -1)
// 		exit_status = get_failure_and_perror("Error while closing write-pipe");
// 	if (waitpid(pipex->pid_child_1, &status, 0) == -1)
// 		exit_status = get_failure_and_perror("Error while waitPID");
// 	if (waitpid(pipex->pid_child_2, &status, 0) == -1)
// 		exit_status = get_failure_and_perror("Error while waitPID");
// 	if (close(pipex->outfile_fd) == -1)
// 		exit_status = get_failure_and_perror("Error while closing output file");
// 	if (exit_status != EXIT_FAILURE)
// 		exit_status = WEXITSTATUS(status);
// 	free_pipex(pipex);
// 	return (exit_status);
// }

// int	init_pipex(t_pipex **pipex, char **argv, char **envp)
// {
// 	int		out_fd;

// 	if (access(argv[1], R_OK) == -1)
// 		perror(argv[1]);
// 	out_fd = open(argv[4], O_TRUNC | O_CREAT | O_RDWR, 0000644);
// 	if (out_fd == -1)
// 		return (perror(argv[4]), (0));
// 	(*pipex) = ft_calloc(1, sizeof(t_pipex));
// 	if (!(*pipex))
// 		return (perror("Memory Allocation failed"), (0));
// 	(*pipex)->argv = argv;
// 	(*pipex)->outfile_fd = out_fd;
// 	(*pipex)->paths = get_paths(envp);
// 	if (!(*pipex)->paths)
// 	{
// 		perror("PATH Error");
// 		close(out_fd);
// 		return (free((*pipex)), (0));
// 	}
// 	if (pipe((*pipex)->pipefd) == -1)
// 		return (perror("Pipe Error"), close(out_fd), free_pipex((*pipex)), (0));
// 	return (1);
// }

int	main(int argc, char **argv, char **envp)
{
	// t_pipex	*pipex;

	// if (argc != 5)
	// 	return (ft_putendl_fd("Invalid Args!", 2), (EXIT_FAILURE));
	// if (!init_pipex(&pipex, argv, envp))
	// 	return (EXIT_FAILURE);
	// execute_first_child(pipex);
	// execute_second_child(pipex);
	// return (clean_up(pipex));
	(void) argc;
	(void) argv;
	(void) envp;

	t_shell shell;
	shell.env = ft_calloc(get_array_size(envp) + 1, sizeof(char *));
	if (!shell.env)
		return (perror("Alloc failed"), (1));
	int i = 0;
	while (i < get_array_size(envp))
	{
		shell.env[i] = ft_strdup(envp[i]);
		i++; 
	}
	shell.env[i] = NULL;
	
	// t_smplcmd pwd_command;
	// char *pwd_args[] = {"pwd", NULL};
	// pwd_command.args = pwd_args;
	// execute_pwd(pwd_command, shell);


	t_smplcmd env_command;
	char *env_args[] = {"env", NULL};
	env_command.args = env_args;
	execute_env(env_command, shell);
	
	// t_smplcmd echo_command;
	// // char *echo_args[] = {"echo", argv[1], argv[2], argv[3], NULL};
	// char *echo_args[] = {"echo", "-n", "Hallo", "Tschau", NULL};
	// echo_command.args = echo_args;
	// execute_echo(echo_command);

	puts("\nExecute CD");
	t_smplcmd cd_command;
	char *cd_args[] = {"cd", "./srcs", NULL};
	cd_command.args = cd_args;
	execute_cd(cd_command, &shell);

	puts("\nEnv after CD\n");
	execute_env(env_command, shell);

	puts("\nExecute CD");
	char *cd_args_2[] = {"cd", "./builtins", NULL};
	cd_command.args = cd_args_2;
	execute_cd(cd_command, &shell);

	puts("\nEnv after CD\n");
	execute_env(env_command, shell);

	puts("\nExecute CD");
	char *cd_args_3[] = {"cd", "..", NULL};
	cd_command.args = cd_args_3;
	execute_cd(cd_command, &shell);

	puts("\nEnv after CD\n");
	execute_env(env_command, shell);

	puts("\nExecute CD");
	char *cd_args_4[] = {"cd", "/nfs/homes/ppfiel/core/03-circle", NULL};
	cd_command.args = cd_args_4;
	execute_cd(cd_command, &shell);

	puts("\nEnv after CD\n");
	execute_env(env_command, shell);
}
