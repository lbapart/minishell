/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 08:39:36 by ppfiel            #+#    #+#             */
/*   Updated: 2023/10/31 14:04:56 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vars	*find_key(char *key, t_vars *env)
{
	int	key_length;

	key_length = ft_strlen(key) + 1;
	while (env)
	{
		if (ft_strncmp(key, env->key, key_length) == 0)
		{
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}

void	free_all_envs(t_vars **env)
{
	t_vars	*temp;
	t_vars	*next;

	if (env && *env)
	{
		temp = *env;
		while (temp)
		{
			next = temp->next;
			if (temp->key)
				free(temp->key);
			if (temp->value)
				free(temp->value);
			free(temp);
			temp = next;
		}
	}
}

void	print_env(t_vars *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void	free_var(t_vars *var)
{
	if (var)
	{
		if (var->key)
			free(var->key);
		if (var->value)
			free(var->value);
		free(var);
	}
}

void	remove_env_by_key(t_vars **env, char *key)
{
	t_vars	*temp;
	t_vars	*next;
	int		key_length;

	key_length = ft_strlen(key) + 1;
	if (*env && ft_strncmp(key, (*env)->key, key_length) == 0)
	{
		temp = *env;
		*env = (*env)->next;
		free_var(temp);
		return ;
	}
	temp = *env;
	while (temp && temp->next)
	{
		next = temp->next;
		if (ft_strncmp(key, next->key, key_length) == 0)
		{
			temp->next = next->next;
			free_var(next);
			return ;
		}
		temp = next;
	}
}
