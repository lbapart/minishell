/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 08:08:53 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/06 08:08:53 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env(t_vars **env, t_vars *new)
{
	if (env)
	{
		if (*env)
			new->next = *env;
		*env = new;
	}
}

void	add_last_env(t_vars **env, t_vars *new)
{
	t_vars	*last;

	if (env)
	{
		if (*env)
		{
			last = *env;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
			*env = new;
	}
}

int	init_env(char **envp, t_shell *shell)
{
	t_vars	*env;
	t_vars	*temp;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		temp = parse_str_to_env(envp[i]);
		if (!temp)
			return (free_all_envs(&env), EXIT_FAILURE);
		add_env(&env, temp);
		i++;
	}
	shell->env = env;
	return (EXIT_SUCCESS);
}

t_vars	*parse_str_to_env(char *str)
{
	char	*key;
	char	*value;
	int		i;
	t_vars	*env;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (ft_putstr_fd("Invalid key-value in ENV!", 2), (NULL));
	key = ft_substr(str, 0, i);
	if (!key)
		return (perror("Allocation failed!"), NULL);
	i++;
	value = ft_substr(str, i, ft_strlen(str));
	if (!value)
		return (perror("Allocation failed!"), free(key), NULL);
	env = new_env(key, value);
	if (!env)
		return (free(key), free(value), NULL);
	return (env);
}

t_vars	*new_env(char *key, char *value)
{
	t_vars	*env;

	env = ft_calloc(1, sizeof(t_vars));
	if (!env)
		return (perror("Allocation failed"), NULL);
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}
