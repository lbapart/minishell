/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:13:01 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/11 19:14:48 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_pwd_to_env(char *keystr, char *value, t_shell *shell)
{
	char	*key;
	t_vars	*pwd_env;

	key = ft_strdup(keystr);
	if (!key)
		return (perror("cd"), EXIT_FAILURE);
	pwd_env = new_env(key, value);
	if (!pwd_env)
		return (free(key), EXIT_FAILURE);
	add_env(&(shell->env), pwd_env);
	return (EXIT_SUCCESS);
}

static void	replace_env_value(t_vars *env, char *value)
{
	free(env->value);
	env->value = value;
}

int	edit_shell_env(t_shell *shell, char *pwd, char *old_pwd)
{
	t_vars	*pwd_env;
	t_vars	*old_pwd_env;

	pwd_env = find_key("PWD", shell->env);
	if (pwd_env)
		replace_env_value(pwd_env, pwd);
	else
	{
		if (add_pwd_to_env("PWD", pwd, shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	old_pwd_env = find_key("OLDPWD", shell->env);
	if (old_pwd_env)
		replace_env_value(old_pwd_env, old_pwd);
	else
	{
		if (add_pwd_to_env("OLDPWD", old_pwd, shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
