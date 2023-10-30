/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:02:00 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/30 19:24:33 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MALLOCEXIT 1
# define NOQUOTES 0;
# define SINGLEQUOTES 1;
# define DOUBLEQUOTES 2;

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
	t_smplcmd	*smplcmd; // every simple command separated by pipe. Do you need counter for simple commands?
	struct s_cmd *next; // next command in history
	struct s_cmd *prev; // previous command in history
} t_cmd;

typedef struct s_vars
{
	size_t			i;
	size_t			j;
	size_t			last_pipe;
	char			*temp;
	char			*res;
	char			*cmd_to_exec;
	t_cmd			*cmds;
	t_cmd			*new_cmd;
	int				is_open_single_quote;
	int				is_open_double_quote;
	t_smplcmd		*smplcmd;
	t_redirection	*redir;
	char			**tokens;
	size_t			tc; // token count
	int				in_quotes; // 0 for no quotes, 1 for single quotes, 2 for double quotes
	char			*start;
	char			*end;
	int				hr; // handle result
} t_vars;

// error.c
void			malloc_err(void);
void			unsupported_char_err(char c);
void			unclosed_quotes_err(void);
void			double_pipe_err(void);
// free.c
void			free_smplcmd(t_smplcmd *smplcmd);
void			free_structs(t_cmd *cmds);
void			free_dbl_ptr(char **ptr);
void			free_and_null(char **str);
void			free_everything(char **tokens, t_cmd *cmd, t_smplcmd *smplcmd);
//list_utils.c
void			lst_cmd_add_back(t_cmd **lst, t_cmd *new);
t_cmd			*lst_cmd_last(t_cmd *lst);
void			lst_redir_add_back(t_redirection **lst, t_redirection *new);
t_redirection	*lst_redir_last(t_redirection *lst);
// parsing.c
void			create_result_token(t_vars *v, char **tokens, t_cmd *t_cmd, char **cmd);
void			copy_until_pipe(char **str_cmd, char *cmd_to_exec, size_t last_pipe, size_t n);
void			replace_vars_with_values(char **tokens, char **str_cmd, t_cmd *cmds);
void			extract_cmd(char **str_cmd, size_t last_pipe, size_t n, t_cmd **cmds);
t_cmd			*parse_commands(char *cmd);
// parsing_init.c
t_smplcmd		*init_simple_command(void);
t_redirection	*init_redir(void);
t_cmd			*init_new_cmd(void);
void			init_vars(t_vars *vars, char *cmd);
// parsing_tokens_2.c
int				add_redir_to_list(t_vars *v, char **tokens);
int				put_token(t_vars *v, char **tokens);
// parsing_tokens.c
int				handle_token(char **start, char **end, char ***tokens, size_t *token_count);
int				handle_redirection(char **start, char **end, char ***tokens, size_t *token_count);
char			**split_command_to_tokens(char *cmd);
t_smplcmd		*put_tokens_to_struct(char **tokens, t_cmd *cmd);
// parsing_utils.c
void			remove_unnecessary_quotes(char *str);
int				check_unclosed_quotes(char *cmd);
int				check_and_put_path(char **tokens, t_smplcmd *smplcmd);
char			*get_var_name(char *var, char **tokens, t_cmd *t_cmd, char **cmd);
//utils.c
int 			is_whitespace(char c);
int 			is_redirection(char c);
int				set_in_quotes_flag(char c, int *in_quotes, char **ptr);
int				is_var_char(char c);
int				is_unsupported_char(char c);



//temp.c
void			print_commands(t_cmd *cmds);

#endif