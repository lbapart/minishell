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

t_vars	*parse_str_to_env(char *str)
{
	char	*key;
	char	*value;
	int		i;

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
		return (perror("Allocation failed!"), NULL);
	return (new_env(key, value));

}

void	free_env(t_vars **env)
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

void	add_env(t_vars **env, t_vars *new)
{
	if (env)
	{
		if (*env)
			new->next = *env;
		*env = new;
	}
}

int	init_env(char **envp, t_shell *shell)
{
	t_vars	*env;
	t_vars	*temp;
	int	i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		temp = parse_str_to_env(envp[i]);
		if (!temp)
			return (free_env(&env), EXIT_FAILURE);
		add_env(&env, temp);
		i++;
	}
	shell->env = env; //TODO: Is it fine, that it is in reversed order in comparison to env? Otherwise change add_env to add_last_env
	return (EXIT_SUCCESS);
}

void	print_env(t_vars *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

