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

typedef struct s_shell
{
	char **env;
} t_shell;


int	execute_cd(t_smplcmd command);
int	execute_pwd(t_smplcmd command, t_shell shell);
int	execute_env(t_smplcmd command, t_shell shell);
int	execute_echo(t_smplcmd command);


int	get_array_size(char **arr);



#endif