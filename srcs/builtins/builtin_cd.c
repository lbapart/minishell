/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:52:38 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/27 11:52:38 by ppfiel           ###   ########.fr       */
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

/*
	TODO: Change cd to use the t_shell env and set PWD and OLDPWD by hand

	- What happend when ENV is not set? --> use chdir or just print out error ?

	Pseudo Code:
	- Check command.args
	- Check if permission on target-directory and if it exists (access(target, X_OK) == 0)
	- Change or create OLDPWD with current pwd
	- Change PWD with target (if relative lookup path)

*/

int	edit_shell_env(t_shell *shell, char *pwd, char *old_pwd)
{
	int	i;
	int	is_pwd_set;
	int	is_old_pwd_set;
	char **new_env; 	//TODO: malloc new env (check if PWD and OLDPWD is contained, otherwise add them)


	is_pwd_set = 0;
	is_old_pwd_set = 0;
	while(shell.env[i])
	{
		if (ft_strncmp("PWD=", shell->env[i], 4) == 0)
		{
			is_pwd_set = 1;
			new_env[i] = ft_strjoin("PWD=", pwd);
			if (!new_env[i])
				return (perror("Allocation failed"), EXIT_FAILURE); //TODO: free_new_env() &&  Just print out error ?
		}
		else if (ft_strncmp("OLDPWD=", shell->env[i], 7) == 0)
		{
			is_old_pwd_set = 1;
			new_env[i] = ft_strjoin("OLDPWD=", old_pwd);
			if (!new_env[i])
				return (perror("Allocation failed"), EXIT_FAILURE); //TODO: free_new_env() Just print out error ?
		}
		i++;
	}
	if (!is_pwd_set)
	{
		new_env[i] = ft_strjoin("PWD=", pwd);
		if (!new_env[i])
			return (perror("Allocation failed"), EXIT_FAILURE); //TODO: free_new_env() Just print out error ?
		i++;
	}
	if (!is_old_pwd_set)
	{
		new_env[i] = ft_strjoin("OLDPWD=", old_pwd);
			if (!new_env[i])
				return (perror("Allocation failed"), EXIT_FAILURE); //TODO: free_new_env() Just print out error ?
	}
	shell->env = new_env;
	return (EXIT_SUCCESS);
}

int	execute_cd(t_smplcmd command, t_shell *shell)
{
	int	error_code;

	if (get_array_size(command.args) != 2)
		return (ft_putendl_fd("Invalid amount of args for cd!", 2), (EXIT_FAILURE)); //TODO: Valid args check in here?
	if (access(path, X_OK) != 0)
	{
		error_code = errno;
		return (perror("Error while accessing directory!"), (error_code)); 
	}
}


// int	execute_cd(t_smplcmd command)
// {
// 	char	*path;
// 	int		error_code;	

// 	if (get_array_size(command.args) != 2)
// 		return (ft_putendl_fd("Invalid amount of args for cd!", 2), (EXIT_FAILURE)); //TODO: Valid args check in here?
// 	path = command.args[1];
// 	if (access(path, X_OK) != 0)
// 	{
// 		error_code = errno;
// 		return (perror("Error while accessing directory!"), (error_code)); 
// 	}
// 	if (chdir(path) != 0)
// 	{
// 		error_code = errno;
// 		return (perror("Error while changing directory!"), (error_code));
// 	}
// 	return (EXIT_SUCCESS);
// }

// #include <string.h>

// void	ft_putendl_fd(char *s, int fd){
// 	puts(s);
// }

// int main(int argc, char **argv)
// {
// 	t_smplcmd cmd;
// 	int			result;
// 	char **args = malloc(sizeof(char *) * 3);
// 	if (!args)
// 		return (EXIT_FAILURE);
// 	args[0] = strdup("cd");
// 	args[1] = argv[1];
// 	args[2] = argv[2];
// 	cmd.args = args;
// 	printf("PWD (in env) before cd: %s\nOLDPWD (in env) before cd: %s\n", getenv("PWD"), getenv("OLDPWD"));
// 	printf("Before cd: Calling pwd builtin: ");
// 	execute_pwd();
// 	result = execute_cd(cmd);
// 	printf("\nPWD (in env) after cd: %s\nOLDPWD (in env) after cd: %s\n", getenv("PWD"), getenv("OLDPWD"));
// 	printf("Before cd: Calling pwd builtin: ");
// 	execute_pwd();
// 	printf("cd return: %d\n", result);
// }