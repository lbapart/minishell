/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 13:43:11 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/04 11:48:03 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_array_size(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}


// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void) argv;

// 	char *line;
// 	t_shell shell;

// 	if (init_env(envp, &shell) != EXIT_SUCCESS)
// 		return (EXIT_FAILURE);
// 	shell.last_exit_code = 0;
// 	shell.exported_vars = NULL;

// 	while (1)
// 	{
// 		line = readline("🤡clownshell🤡$ ");
// 		if (line == NULL)
// 		{
// 			break;
// 		}
// 		if (line[0] != '\0')
// 			add_history(line);
// 		exec_commands(line, &shell);
// 		free(line);
// 		//rl_redisplay();
// 		//rl_on_new_line();
// 	}
// 	clear_history();
// 	return (0);
// }

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *cmd;
	t_cmd *cmds;
	t_shell shell;
	if (init_env(envp, &shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	shell.exported_vars = NULL;
	shell.last_exit_code = 0;

	cmd = ft_strdup("echo $?");
	if (!cmd)
		return (0);
	cmds = parse_commands(cmd, &shell);
	//printf("saassasaas\n");
	print_commands(cmds);
	free_structs(&cmds);
	free(cmd);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void) argc;
// 	(void) argv;
// 	(void) envp;

// 	// int i = 0;
// 	// while (envp[i])
// 	// 	printf("%s\n", envp[i++]);
// 	t_shell shell;
// 	if (init_env(envp, &shell) != EXIT_SUCCESS)
// 		return (EXIT_FAILURE);
// 	shell.exported_vars = NULL;
// //	t_vars *temp = shell.exported_vars;
// //	while (temp)
// //	{
// //		printf("%s='%s' (next=%p)\n", temp->key, temp->value, temp->next);
// //		temp = temp->next;
// //	}

// 	// t_smplcmd env_command;
// 	// char *env_args[] = {"env", NULL};
// 	// env_command.args = env_args;
// 	// execute_env(env_command, shell);

// 	// t_smplcmd pwd_command;
// 	// char *pwd_args[] = {"pwd", NULL};
// 	// pwd_command.args = pwd_args;
// 	// execute_pwd(pwd_command);

// 	// t_smplcmd echo_command;
// 	// // char *echo_args[] = {"echo", argv[1], argv[2], argv[3], NULL};
// 	// char *echo_args[] = {"echo", "-n", "Hallo", "Tschau", NULL};
// 	// echo_command.args = echo_args;
// 	// execute_echo(echo_command);

// 	// puts("\nExecute CD");
// 	// t_smplcmd cd_command;
// 	// char *cd_args[] = {"cd", "../03-minishell/srcs/./builtins", NULL};
// 	// cd_command.args = cd_args;
// 	// printf("Error Code cd: %d\n", execute_cd(cd_command, &shell));
// 	// execute_pwd(pwd_command);

// 	puts("\nEXECUTE ENV:\n");

// 	t_smplcmd env_command;
// 	char *env_args[] = {"env", NULL};
// 	env_command.args = env_args;
// 	execute_env(env_command, shell);
// //	temp = shell.exported_vars;
// //	while (temp)
// //	{
// //		printf("%s='%s' (next=%p)\n", temp->key, temp->value, temp->next);
// //		temp = temp->next;
// //	}

// 	puts("\nEXPORT without parameter\n");
// 	t_smplcmd export_no_args;
// 	char *export_args[] = {"export", NULL};
// 	export_no_args.args = export_args;
// 	execute_export(&shell, export_no_args);
// //	temp = shell.exported_vars;
// //	while (temp)
// //	{
// //		printf("%s='%s' (next=%p)\n", temp->key, temp->value, temp->next);
// //		temp = temp->next;
// //	}

// 	puts("\nEXPORT with 1 Parameter without value\n");
// 	t_smplcmd export_one_param_no_value;
// 	char *export_one_param_no_valueargs[] = {"export", "hasNoValue", NULL};
// 	export_one_param_no_value.args = export_one_param_no_valueargs;
// 	printf("Exit-Code: %d\n", execute_export(&shell, export_one_param_no_value));
// 	printf("Exit-Code: %d\n", execute_export(&shell, export_one_param_no_value));
// //	temp = shell.exported_vars;
// //	while (temp)
// //	{
// //		printf("%s='%s' (next=%p)\n", temp->key, temp->value, temp->next);
// //		temp = temp->next;
// //	}


// 	puts("\nEXPORT without parameter\n");
// 	execute_export(&shell, export_no_args);

// 	puts("\nEXECUTE ENV:\n");
// 	execute_env(env_command, shell);

// 	puts("\nEXECUTE unset without parameter");
// 	t_smplcmd unset_no_param;
// 	char *unset_no_valueargs[] = {"unset", NULL};
// 	unset_no_param.args = unset_no_valueargs;
// 	printf("Exit-Code: %d\n", execute_unset(unset_no_param, &shell));

// 	puts("\nEXPORT without parameter\n");
// 	execute_export(&shell, export_no_args);

// 	puts("\nEXECUTE unset hasNoValue parameter");
// 	t_smplcmd unset_hasnotexported;
// 	char *unset_hasnotexported_args[] = {"unset", "hasNoValue", NULL};
// 	unset_hasnotexported.args = unset_hasnotexported_args;
// 	printf("Exit-Code: %d\n", execute_unset(unset_hasnotexported, &shell));

// 	puts("\nEXPORT without parameter\n");
// 	execute_export(&shell, export_no_args);

// 	puts("\nEXECUTE unset HOME parameter");
// 	t_smplcmd unset_home;
// 	char *unset_home_args[] = {"unset", "HOME", NULL};
// 	unset_home.args = unset_home_args;
// 	printf("Exit-Code: %d\n", execute_unset(unset_home, &shell));

// 	puts("\nEXPORT without parameter\n");
// 	execute_export(&shell, export_no_args);

// 	puts("\nEXECUTE unset USER notexportedyed SHLVL parameter");
// 	t_smplcmd unset_multiple;
// 	char *unset_multiple_args[] = {"unset", "USER", "notexportedyed", "SHLVL", NULL};
// 	unset_multiple.args = unset_multiple_args;
// 	printf("Exit-Code: %d\n", execute_unset(unset_multiple, &shell));

// 	puts("\nEXPORT without parameter\n");
// 	execute_export(&shell, export_no_args);

// 	puts("\nEXECUTE exit no parameter");
// 	t_smplcmd exit_no_args;
// 	char *exit_no_args_args[] = {"exit", NULL};
// 	exit_no_args.args = exit_no_args_args;
// 	printf("Exit-Code: %d\n", execute_exit(exit_no_args));

// 	puts("\nEXECUTE exit 1 parameter: exit 5");
// 	t_smplcmd exit_one_arg;
// 	char *exit_one_arg_args[] = {"exit", "5", NULL};
// 	exit_one_arg.args = exit_one_arg_args;
// 	printf("Exit-Code: %d\n", execute_exit(exit_one_arg));

// 	puts("\nEXECUTE exit multiple parameter: Invalid --> 1");
// 	t_smplcmd exit_multiple_args;
// 	char *exit_multiple_args_args[] = {"exit", "5", "3", NULL};
// 	exit_multiple_args.args = exit_multiple_args_args;
// 	printf("Exit-Code: %d\n", execute_exit(exit_multiple_args));

// 	puts("\nEXECUTE exit non-numeric parameter: Invalid: numeric argument required --> 2");
// 	t_smplcmd exit_non_numeric;
// 	char *exit_non_numeric_args[] = {"exit", "5a5", NULL};
// 	exit_non_numeric.args = exit_non_numeric_args;
// 	printf("Exit-Code: %d\n", execute_exit(exit_non_numeric));

// 	puts("\nEXECUTE exit 1 parameter with unsigned char overflow: exit -1000 --> 24 ");
// 	t_smplcmd exit_overflow;
// 	char *exit_overflow_args[] = {"exit", "-1000", NULL};
// 	exit_overflow.args = exit_overflow_args;
// 	printf("Exit-Code: %d\n", execute_exit(exit_overflow));

// 	// puts("\nExport without parameter\n");
// 	// t_smplcmd export_command;
// 	// char *export_args[] = {"export", NULL};
// 	// export_command.args = export_args;
// 	// execute_export(&shell, export_command);

// 	// puts("\nEnv after CD\n");
// 	// execute_env(env_command, shell);

// 	// puts("\nExecute CD");
// 	// char *cd_args_2[] = {"cd", "./builtins", NULL};
// 	// cd_command.args = cd_args_2;
// 	// execute_cd(cd_command, &shell);

// 	// puts("\nEnv after CD\n");
// 	// execute_env(env_command, shell);

// 	// puts("\nExecute CD");
// 	// char *cd_args_3[] = {"cd", "..", NULL};
// 	// cd_command.args = cd_args_3;
// 	// execute_cd(cd_command, &shell);

// 	// puts("\nEnv after CD\n");
// 	// execute_env(env_command, shell);

// 	// puts("\nExecute CD");
// 	// char *cd_args_4[] = {"cd", "/nfs/homes/ppfiel/core/03-circle", NULL};
// 	// cd_command.args = cd_args_4;
// 	// execute_cd(cd_command, &shell);

// 	// puts("\nEnv after CD\n");
// 	// execute_env(env_command, shell);
// }