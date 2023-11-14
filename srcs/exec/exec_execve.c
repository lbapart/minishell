/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:32:55 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/14 14:39:37 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_entry_str(t_vars *env)
{
	int		i;
	int		j;
	char	*env_entry;

	i = 0;
	j = 0;
	env_entry = ft_calloc(2 + ft_strlen(env->key) + ft_strlen(env->value), 1);
	if (!env_entry)
		return (NULL);
	while (env->key[j])
		env_entry[i++] = env->key[j++];
	env_entry[i++] = '=';
	j = 0;
	while (env->value[j])
		env_entry[i++] = env->value[j++];
	env_entry[i] = '\0';
	return (env_entry);
}

static void	free_env_arr(char ***env_as_char_arr)
{
	int	i;

	i = 0;
	if (*env_as_char_arr)
	{
		while ((*env_as_char_arr)[i])
			free((*env_as_char_arr)[i++]);
		free((*env_as_char_arr));
	}
}

static char	**get_env_as_char_arr(t_shell *shell)
{
	int		size;
	t_vars	*env;
	char	**env_as_char_arr;
	int		i;

	env = shell->env;
	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	env_as_char_arr = ft_calloc(size + 1, sizeof(char *));
	if (!env_as_char_arr)
		return (perror("Allocation failed"), NULL);
	env = shell->env;
	i = 0;
	while (env)
	{
		env_as_char_arr[i] = get_env_entry_str(env);
		if (!env_as_char_arr[i++])
			return (free_env_arr(&env_as_char_arr), NULL);
		env = env->next;
	}
	return (env_as_char_arr);
}

int	exec_simple_command(t_smplcmd *smplcmd, t_shell *shell)
{
	char	*path;
	char	**args;
	char	**env;

	args = smplcmd->args;
	path = smplcmd->path;
	init_signals(CHILD_MODE);
	if (access(path, X_OK) != 0 || ft_strlen(smplcmd->args[0]) == 0
		|| (ft_strncmp(smplcmd->args[0], ".", 2)) == 0)
	{
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	env = get_env_as_char_arr(shell);
	if (!env)
		return (EXIT_FAILURE);
	if (execve(path, args, env) == -1)
		return (perror("execve"), free_env_arr(&env), 1);
	return (free_env_arr(&env), EXIT_FAILURE);
}