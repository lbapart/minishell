/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbapart <lbapart@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:50:26 by lbapart           #+#    #+#             */
/*   Updated: 2023/10/24 18:11:01 by lbapart          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_smplcmd
{
	char	*cmd;
	char	**args;
	char	*bultin;
	char	*redirect;
} t_smplcmd;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	char *input;
	char *output;
	t_smplcmd *smplcmd;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd;


void	free_and_null(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

int	is_open_double_quote(char *cmd, size_t n)
{
	int is_quote_open;
	size_t  i;

	i = 0;
	is_quote_open = 0;
	while (cmd[i] && i < n)
	{
		if (cmd[i] == '\"' && !is_quote_open)
			is_quote_open = 1;
		else if (cmd[i] == '\"' && is_quote_open)
			is_quote_open = 0;
	}
}

int	is_open_single_quote(char *cmd, size_t n)
{
	int is_quote_open;
	size_t i;

	i = 0;
	is_quote_open = 0;
	while (cmd[i] && i < n)
	{
		if (cmd[i] == '\'' && !is_quote_open)
			is_quote_open = 1;
		else if (cmd[i] == '\'' && is_quote_open)
			is_quote_open = 0;
	}
}

void	delete_parsed_cmd(char **cmd, size_t n)
{
	char *new_cmd;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	new_cmd = (char *)malloc(sizeof(char) * ft_strlen(*cmd) - n + 1);
	if (!new_cmd)
		return (free_and_null(cmd), NULL); // throw error here and free everything and exit
	while ((*cmd)[i] && i < n)
		i++;
	i++;
	if (!(*cmd)[i])
		return (free_and_null(cmd), NULL);
	while ((*cmd)[i])
	{
		new_cmd[j] = (*cmd)[i];
		i++;
		j++;
	}
	new_cmd[j] = '\0';
	free_and_null(cmd);
	*cmd = new_cmd;
}

void	parse_command(char *cmd)
{
	size_t i;
	char	**splitted_by_space;
	
	i = 0;
	splitted_by_space = ft_split(cmd, " ");
	if (!splitted_by_space)
		return (NULL);
	while (splitted_by_space[i])
	{
		
	}
}

void	extract_cmd(char **cmd, size_t n)
{
	char *cmd_to_exec;
	size_t i;

	i = 0;
	cmd_to_exec = malloc(sizeof(char) * n + 1);
	if (!cmd_to_exec)
		return (free_and_null(cmd), NULL); // throw error here and free everything and exit
	while ((*cmd) && (*cmd)[i] && i < n)
	{
		cmd_to_exec[i] = (*cmd)[i];
		i++;
	}
	cmd_to_exec[i] = '\0';
	delete_parsed_cmd(cmd, n);
	parse_command(cmd_to_exec);
}

void	init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
}

void parse_commands(char *cmd)
{
	size_t i;
	size_t last_pipe;
	t_cmd *cmds;

	i = 0;
	last_pipe = 0;
	
	while (cmd && cmd[i])
	{
		if (cmd[i] == '|' && !is_open_single_quote(cmd, i) && !is_open_double_quote(cmd, i))
		{
			extract_cmd(&cmd, i);
		}
		i++;
	}
}
