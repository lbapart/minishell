/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:25:05 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/27 15:25:05 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h> // TODO: Are we allowed to use it?

# define ERROR_FATAL 4200
# define ERROR_INVALID_ARGS 4201

typedef struct s_smplcmd
{
	char	*cmd; // do we need this? no i guess
	char	**args; // args for execve
	char	*bultin; // if it's a builtin command
	char	*input;  // input file
	int		input_redirection; // 0 for no redirection, 1 for <, 2 for << need to define macros
	char	*output; // output file
	int		output_redirection; // 0 for no redirection, 1 for >, 2 for >> need to define macros
} t_smplcmd;

typedef struct s_cmd
{
	char *cmd; // cmd to execute for history
	int	signal_received; // 0 for no signal, 1 for SIGINT, 2 for SIGQUIT can we put it here and use this struct as global? 
						// allocate and init it in main. after signal received set it to 1 or 2. then do signal stuff, and set it back to 0 
	t_smplcmd	*smplcmd; // every simple command separated by pipe. Do you need counter for simple commands?
	struct s_cmd *next; // next command in history
	struct s_cmd *prev; // previous command in history
} t_cmd;

typedef struct s_vars
{
	char	*key;
	char	*value;
	struct s_vars	*next;
} t_vars;


typedef struct s_shell
{
	t_vars 	*env;
	t_vars	*exported_vars;
} t_shell;



int	execute_cd(t_smplcmd command, t_shell *shell);
int	execute_pwd(t_smplcmd command);
int	execute_env(t_smplcmd command, t_shell shell);
int	execute_echo(t_smplcmd command);
int	execute_export(t_shell *shell, t_smplcmd command);


int	get_array_size(char **arr);
int	init_env(char **envp, t_shell *shell);
t_vars	*find_key(char *key, t_vars *env);
void	add_env(t_vars **env, t_vars *new);
t_vars	*new_env(char *key, char *value);
int	get_pwd_string(char **pwd);

void	free_env(t_vars **env);
void	print_env(t_vars *env);

#endif