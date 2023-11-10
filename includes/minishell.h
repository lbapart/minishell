/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:25:05 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/04 11:47:41 by ppfiel           ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>

# define ERROR_FATAL 4200
# define ERROR_INVALID_ARGS 4201
# define MALLOCEXIT 1
# define NOQUOTES 0
# define SINGLEQUOTES 1
# define DOUBLEQUOTES 2
# define NOBUILTIN 0
# define ECHO 1
# define CD 2
# define PWD 3
# define EXPORT 4
# define UNSET 5
# define ENV 6
# define EXIT 7
# define HIDDEN_QUOTE -20
# define HEREDOC_HIDDEN_DOLLAR -21
# define REDIR_HIDDEN_DOLLAR -22

typedef struct s_redirection
{
	char					*file;
	int						type; // 0 for no redirection, 1 for <, 2 for <<, 3 for >, 4 for >>
	struct s_redirection	*next;
} t_redirection;

typedef struct s_smplcmd
{
	char			*path; // path to executable
	char			**args; // args for execve
	int				builtin; // if it's a builtin command
	t_redirection	*redir; // input file
} t_smplcmd;

typedef struct s_cmd
{
	pid_t			pid;
	int				pipe[2];
	t_smplcmd		*smplcmd; // every simple command separated by pipe. Do you need counter for simple commands?
	struct s_cmd	*next; // next command in history
	struct s_cmd	*prev; // previous command in history
	struct s_shell	*shell;
} t_cmd;

typedef struct s_vars
{
	char	*key;
	char	*value;
	struct s_vars	*next;
} t_vars;


typedef struct s_pars_vars
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
	struct s_shell	*shell;
	int				iosq;
	int				iodq;
} t_pars_vars;

typedef struct s_shell
{
	t_vars 	*env;
	t_vars	*exported_vars;
	int		last_exit_code;
	int		is_exit;
} t_shell;


// builtins:
int		execute_cd(t_smplcmd command, t_shell *shell);
int		execute_pwd(void);
int		execute_env(t_smplcmd command, t_shell shell);
int		execute_echo(t_smplcmd command);
int		execute_export(t_shell *shell, t_smplcmd command);
int		execute_unset(t_smplcmd command, t_shell *shell);
int		execute_exit(t_smplcmd command, t_shell *shell);

int		get_array_size(char **arr);
int		init_env(char **envp, t_shell *shell);
t_vars	*find_key(char *key, t_vars *env);
void	add_env(t_vars **env, t_vars *new);
t_vars	*new_env(char *key, char *value);
int		get_pwd_string(char **pwd);

void	free_all_envs(t_vars **env);
void	print_env(t_vars *env);
void	remove_env_by_key(t_vars **env, char *key);

void    add_last_env(t_vars **env, t_vars *new);

// builtin_cd_utils.c
int	edit_shell_env(t_shell *shell, char *pwd, char *old_pwd);

// builtin_export_utils.c
void	print_export(t_vars *vars);
t_vars	*sort_vars(t_vars *vars);
int		copy_vars(t_vars **dest, t_vars *src);
void	swap_vars(t_vars *first, t_vars *second);
int		handle_add_to_env(char *key, char *value, t_shell *t_shell);

// env_create.c
void	add_env(t_vars **env, t_vars *new);
void	add_last_env(t_vars **env, t_vars *new);
int		init_env(char **envp, t_shell *shell);
t_vars	*parse_str_to_env(char *str);
t_vars	*new_env(char *key, char *value);

// exec.c
void			exec_commands(char *cmd, t_shell *shell);
int				exec_simple_command(t_smplcmd *smplcmd, t_shell *shell);
int				exec_builtin(t_smplcmd *smplcmd, t_shell *shell);
// parsing_error_2.c
void			unexpected_near_pipe_err(void);
void			amb_redir_err(void);
// parsing_error.c
void			malloc_err(void);
void			unsup_char_err(char c);
void			unclosed_quotes_err(void);
void			double_pipe_err(void);
void			redir_token_err(void);
// parsing_free_2.c
void			free_extract_cmd_2(t_cmd **cmds, t_pars_vars *v);
void			free_extract_cmd_3(char **str_cmd, t_shell *shell);
void			free_ext_cmd4_and_err(t_cmd **cmds, t_smplcmd *smplcmd,
					char **str, t_shell *shell);
void			free_if_question(char *key, char *var_value);
void			free_replace_vars(t_cmd **cmds, char **str_cmd, t_pars_vars *v);
// parsing_free_3.c
void			free_get_var_name_and_err(t_cmd **struct_cmd, char **cmd,
					t_pars_vars *v);
void			free_extract_cmd(t_cmd **cmds, char **str_cmd, t_shell *shell);
// parsing_free.c
void			free_smplcmd(t_smplcmd *smplcmd);
void			free_structs(t_cmd **cmds);
void			free_dbl_ptr(char **ptr);
void			free_and_null(char **str);
void			free_everything(char **tokens, t_cmd *cmd, t_smplcmd *smplcmd);
// parsing_finish.c
t_cmd			*finish_pars(t_cmd *cmd, t_shell *shell, char *strcmd);
void			set_builtin(t_cmd *cmd);
char			**finish_split_tokens(char *cmd, t_pars_vars *v, char **temp);
// parsing_list_utils.c
void			lst_cmd_add_back(t_cmd **lst, t_cmd *new);
t_cmd			*lst_cmd_last(t_cmd *lst);
void			lst_redir_add_back(t_redirection **lst, t_redirection *new);
t_redirection	*lst_redir_last(t_redirection *lst);
// parsing.c
void			create_result_command(t_pars_vars *v, t_pars_vars *in_v, char *var_value);
void			set_prev_cmds(t_cmd *cmds);
void			replace_vars_with_values(char **str_cmd, t_pars_vars *v, t_cmd **cmds, t_shell *shell);
int				extract_cmd(char **str_cmd, t_pars_vars *p, t_shell *shell);
t_cmd			*parse_commands(char *cmd, t_shell *shell);
// parsing_init.c
t_smplcmd		*init_simple_command(void);
t_redirection	*init_redir(void);
t_cmd			*init_new_cmd(t_shell *shell);
void			init_vars(t_pars_vars *vars, char *cmd, t_shell *shell);
// parsing_redirections.c
int				is_redir_token(char *token);
int				is_valid_token_for_redir(char *token);
int				check_redir_tokens(char **tokens);
int				ft_strcmp(char *s1, char *s2);
// parsing_tokens_2.c
int				add_redir_to_list(t_pars_vars *v, char **tokens);
int				put_token(t_pars_vars *v, char **tokens);
// parsing_tokens.c
int				handle_token(char **start, char **end, char ***tokens, size_t *token_count);
int				handle_redirection(char **start, char **end, char ***tokens, size_t *token_count);
char			**split_command_to_tokens(char *cmd);
t_smplcmd		*put_tokens_to_struct(char **tokens, t_cmd *cmd);
int				put_path(t_smplcmd *smplcmd);
// parsing_utils.c
void			remove_unnecessary_quotes(char *str);
void			set_in_quotes_var(char c, int *in_quotes);
int				check_heredoc_before(char *cmd, size_t n);
int				check_redirection_before(char *cmd, size_t n);
int				check_unclosed_quotes(char *cmd);
char			*get_var_name(char *var, t_cmd **t_cmd, char **cmd, t_pars_vars *vars);
// parsing_utils_2.c
void			copy_until_pipe(char **str_cmd, char *cmd_to_exec,
					size_t last_pipe, size_t n);
void			set_hidden_quotes(char *cmd, size_t len);
// utils.c
int 			is_whitespace(char c);
int 			is_redirection(char c);
int				set_in_quotes_flag(char c, int *in_quotes, char **ptr);
int				is_var_char(char c);
int				is_unsupported_char(char c);
// heredoc.c
char			*generate_filename(int pid);
void			replace_redir_filename(t_redirection *redir, char *filename);
int				exec_heredoc(t_redirection *redir, int pid);
// parsing_vars_2.c
char	*get_var_name_str(char *str, t_shell *shell, t_cmd *cmd, char *strcmd);
char	*get_var_value_str(char *var_name, t_shell *shell, t_cmd *cmd, char *strcmd);
char	*replace_var_with_value(char **str, char *var_value, char *var_name, size_t pos);
char	*replace_vars_in_str(char **str, t_shell *shell, t_cmd *cmd, char *strcmd);
int		check_whitespace(char *str);
int 	replace_vars_in_redir(t_redirection *redir, t_shell *shell, t_cmd *cmd, char *strcmd);
t_cmd	*replace_all_vars_redir(t_cmd *cmd, t_shell *shell, char *strcmd);
// path_utils.c
char	*ft_not_free_strjoin(char const *s1, char const *s2);
char	**get_paths(t_vars *env, t_cmd *cmd, t_shell *shell, char *strcmd);
char	*get_command_path(char **paths, char *command_name);
t_cmd   *replace_with_absolute_path(t_cmd *cmd, t_shell *shell, char *strcmd);
//temp.c
void			print_commands(t_cmd *cmds);

#endif