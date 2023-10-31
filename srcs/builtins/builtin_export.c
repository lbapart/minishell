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

void	print_export(t_vars *vars)
{
	while (vars)
	{
		printf("declare -x %s=\"%s\"\n",  vars->key, vars->value);
		vars = vars->next;
	}
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

t_vars *sort_list(t_vars *vars)
{
	t_vars	*tmp;

	tmp = vars;
	while(vars->next != NULL)
	{
		if (ft_strncmp(vars->key, vars->next->key, ft_strlen(vars->key) + 1) > 0)
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

	*dest = NULL;
	while (src)
	{
		temp = new_env(src->key, src->value);
		if (!temp)
			return (free_env(dest), ERROR_FATAL);
		add_env(dest, temp);
		src = src->next;
	}
	return (EXIT_SUCCESS);
}

int	join_env_and_exported_vars(t_shell *shell)
{
	t_vars	*joined;
	t_vars	*temp_env;
	t_vars	*temp_exported;

	if (copy_vars(&temp_env, shell->env) != EXIT_SUCCESS)
			return (ERROR_FATAL);
	if (shell->exported_vars)
	{
		if (copy_vars(&temp_exported, shell->exported_vars) != EXIT_SUCCESS)
			return (free_env(&joined), (ERROR_FATAL));
		add_env(&temp_env, temp_exported);
	}
	temp_env = sort_list(temp_env);
	print_export(temp_env);
	return (EXIT_SUCCESS);
}

// int	handle_printing_sorted_env(t_shell *shell)
// {
	
// }

// int	handle_key_value_pair(char *key_value_pair, t_shell *shell)
// {

// }

// t_vars	*find_variable_in_list(t_vars *vars, char *variable)
// {
// 	int	var_length;

// 	var_length = ft_strlen(variable);
// 	while (vars)
// 	{
// 		if (ft_strlen(vars->key) == var_length && ft_strncmp(vars->key, variable, var_length))
// 			return (vars);
// 		vars = vars->next;
// 	}
// 	return (NULL);
// }

// int	join_key_value(t_vars *key_value_pair, char **key_value_str)
// {
// 	char	*joined;
// 	int		size;
// 	int		i;
// 	int		j;
// 	int		error_code;

// 	size = ft_strlen(key_value_pair->key) + ft_strlen(key_value_pair->value) + 2;
// 	joined = ft_calloc(size, 1);
// 	if (!joined)
// 	{
// 		error_code = errno;
// 		return (perror("Allocation failed"), error_code);
// 	}
// 	i = 0;
// 	while (key_value_pair->key[i])
// 	{
// 		joined[i] = key_value_pair->key[i];
// 		i++;
// 	}
// 	joined[i++] = '=';
// 	j = 0;
// 	while (key_value_pair->value[j])
// 	{
// 		joined[j + i] = key_value_pair->value[j];
// 		j++;
// 	}
// 	joined[i + j] = '\0';
// 	*key_value_str = joined;
// 	return (EXIT_SUCCESS);
// }

// int	handle_variable_lookup(char *variable, t_shell *shell)
// {
// 	char	**new_env;
// 	int		error_code;
// 	int		i;
// 	char	*key_value_str;
// 	t_vars	*variable_in_list;

// 	variable_in_list = find_variable_in_list(shell->vars, variable);
// 	if (variable_in_list)
// 	{
// 		error_code = join_key_value(variable_in_list, &key_value_str);
// 		if (error_code != EXIT_SUCCESS)
// 			return (error_code);
// 		new_env = ft_calloc(get_array_size(shell->env) + 2, sizeof(char *));
// 		if (!new_env)
// 		{
// 			error_code = errno;
// 			return (perror("Allocation failed"), error_code);
// 		}
// 		i = 0;
// 		while (shell->env[i])
// 		{
// 			new_env = shell->env[i];
// 			i++;
// 		}
// 		new_env[i] = key_value_str;
// 	}
// 	return (EXIT_SUCCESS);
// }

int	execute_export(t_shell *shell, t_smplcmd command)
{
	int	i;

	if (get_array_size(command.args) == 1)
	{
		return (join_env_and_exported_vars(shell));
		//export without parameter: sorted env with prefix
	}
	i = 1;
	while (command.args[i])
	{
		if (ft_strchr(command.args[i], '='))
		{
			// Handle splitting key - value and add to env
		}
		else
		{
			//Handle Variable lookup and if contained add to env
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
