/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:50:01 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/31 14:16:03 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env_and_exported(t_shell *shell)
{
	t_vars	*temp_env;
	t_vars	*temp_exported;

	if (copy_vars(&temp_env, shell->env) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (shell->exported_vars)
	{
		if (copy_vars(&temp_exported, shell->exported_vars) != EXIT_SUCCESS)
			return (free_all_envs(&temp_env), (EXIT_FAILURE));
		add_last_env(&temp_env, temp_exported);
	}
	temp_env = sort_vars(temp_env);
	print_export(temp_env);
	free_all_envs(&temp_env);
	return (EXIT_SUCCESS);
}

int	handle_add_replace_to_env(char *str, t_shell *shell, int equals_index)
{
	char	*key;
	char	*value;
	t_vars	*old_env;

	key = ft_substr(str, 0, equals_index);
	if (!key)
		return (perror("Allocation failed"), EXIT_FAILURE);
	value = ft_substr(str, equals_index + 1, ft_strlen(str) - equals_index + 1);
	if (!value)
		return (perror("Allocation failed"), free(key), EXIT_FAILURE);
	old_env = find_key(key, shell->env);
	if (old_env)
	{
		free(old_env->value);
		old_env->value = value;
		free(key);
	}
	else
		return (handle_add_to_env(key, value, shell));
	return (EXIT_SUCCESS);
}

int	handle_add_to_exported(char *str, t_shell *shell)
{
	t_vars	*to_add;
	char	*key;
	char	*value;

	if (!find_key(str, shell->exported_vars) && !find_key(str, shell->env))
	{
		key = ft_strdup(str);
		if (!key)
			return (perror("Allocation failed"), EXIT_FAILURE);
		value = NULL;
		to_add = new_env(key, value);
		if (!to_add)
		{
			perror("Allocation failed");
			return (free(key), free(value), EXIT_FAILURE);
		}
		add_env(&(shell->exported_vars), to_add);
	}
	return (EXIT_SUCCESS);
}

int	handle_variable_assignment(char *arg, t_shell *shell)
{
	char	*equal_address;
	int		equal_pos;

	equal_address = ft_strchr(arg, '=');
	if (equal_address)
	{
		equal_pos = equal_address - arg;
		if (handle_add_replace_to_env(arg, shell, equal_pos) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (-1);
}

int	execute_export(t_shell *shell, t_smplcmd command)
{
	int		i;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	if (get_array_size(command.args) == 1)
		return (print_env_and_exported(shell));
	i = 1;
	while (command.args[i])
	{
		if (!is_valid_key(command.args[i]))
			exit_code = EXIT_FAILURE;
		else
		{
			if (!handle_export(command.args[i], shell, &exit_code))
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (exit_code);
}
