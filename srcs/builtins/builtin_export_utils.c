/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 08:23:26 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/06 08:23:26 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_vars *vars)
{
	while (vars)
	{
		if (vars->value)
			printf("declare -x %s=\"%s\"\n", vars->key, vars->value);
		else
			printf("declare -x %s\n", vars->key);
		vars = vars->next;
	}
}

t_vars	*sort_vars(t_vars *vars)
{
	t_vars	*tmp;

	tmp = vars;
	while (vars && vars->next != NULL)
	{
		if (ft_strncmp(vars->key, vars->next->key,
				ft_strlen(vars->key) + 1) > 0)
		{
			swap_vars(vars, vars->next);
			vars = tmp;
		}
		else
			vars = vars->next;
	}
	vars = tmp;
	return (vars);
}

int	copy_vars(t_vars **dest, t_vars *src)
{
	t_vars	*temp;
	char	*key;
	char	*value;

	*dest = NULL;
	while (src)
	{
		key = ft_strdup(src->key);
		if (!key)
			return (free_all_envs(dest), EXIT_FAILURE);
		if (src->value)
		{
			value = ft_strdup(src->value);
			if (!value)
				return (free_all_envs(dest), free(key), EXIT_FAILURE);
		}
		else
			value = NULL;
		temp = new_env(key, value);
		if (!temp)
			return (free_all_envs(dest), free(key), free(value), EXIT_FAILURE);
		add_env(dest, temp);
		src = src->next;
	}
	return (EXIT_SUCCESS);
}

void	swap_vars(t_vars *first, t_vars *second)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = first->key;
	temp_value = first->value;
	first->key = second->key;
	first->value = second->value;
	second->key = temp_key;
	second->value = temp_value;
}

int	handle_add_to_env(char *key, char *value, t_shell *shell)
{
	t_vars	*to_add;
	t_vars	*exported_env;

	to_add = new_env(key, value);
	if (!to_add)
	{
		perror("Allocation failed");
		return (free(key), free(value), EXIT_FAILURE);
	}
	add_env(&shell->env, to_add);
	exported_env = find_key(key, shell->exported_vars);
	if (exported_env)
		remove_env_by_key(&(shell->exported_vars), key);
	return (EXIT_SUCCESS);
}
