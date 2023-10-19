/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:58:47 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/13 09:33:55 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include "../libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_command
{
	char	*name;
	char	**args;
	char	*path;
	int		status;
}	t_command;

typedef struct s_pipex
{
	int			pipefd[2];
	int			outfile_fd;
	char		**argv;
	char		**paths;
	pid_t		pid_child_1;
	pid_t		pid_child_2;
}	t_pipex;

typedef struct s_execute_command_data
{
	t_command	*command;
	int			unused_pipe;
	int			used_pipe;
	int			file_fd;
	char		*filename;
	int			input_redirect;
	int			output_redirect;
}	t_execute_command_data;

// execute_commands.c
void		execute_command(t_pipex *pex, t_execute_command_data data);

// command_utils.c
t_command	*parse_command(char *str, char **paths);
void		free_command(t_command *cmd);

// path_utils.c
char		**get_paths(char **env);
char		*get_command_path(char **paths, char *command_name);

// error.c
void		close_pipe(int pipe[]);
void		handle_command_error(t_pipex *pipex, char *command_name, int fd);
void		handle_redirect_error(int file, int pipe, t_pipex *p, t_command *c);
void		free_pipex(t_pipex *pipex);

//execute_child.c
void		execute_first_child(t_pipex *pipex);
void		execute_second_child(t_pipex *pipex);

#endif