/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:02:00 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/28 18:29:33 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_redirection
{
	char	*file;
	int		type; // 0 for no redirection, 1 for <, 2 for <<, 3 for >, 4 for >>
	struct s_redirection *next;
} t_redirection;

typedef struct s_smplcmd
{
	char	*cmd; // do we need this? no i guess
	char	*path; // path to executable
	char	**args; // args for execve
	char	*bultin; // if it's a builtin command
	t_redirection	*redir; // input file
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

#define MALLOCEXIT 1

//list_utils.c
void	lst_cmd_add_back(t_cmd **lst, t_cmd *new);
t_cmd	*lst_cmd_last(t_cmd *lst);
void	lst_redir_add_back(t_redirection **lst, t_redirection *new);
t_redirection	*lst_redir_last(t_redirection *lst);
// parsing.c

