/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppfiel <ppfiel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 13:43:11 by ppfiel            #+#    #+#             */
/*   Updated: 2023/11/13 09:07:43 by ppfiel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" //TODO: change back

int	get_array_size(char **arr)
{
	int	size;

	size = 0;
	while (arr[size])
		size++;
	return (size);
}

static int	is_valid_shell_lvl(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
		{
			ft_putstr_fd("Invalid SHLVL: ", 2);
			ft_putendl_fd(str, 2);
			return (0);
		}
	}
	if (ft_strlen(str) > 10
		|| (ft_strlen(str) == 10 && ft_strncmp(str, "2147483646", 11) > 0))
	{
		ft_putstr_fd("Invalid SHLVL: ", 2);
		ft_putendl_fd(str, 2);
		return (0);
	}
	return (1);
}

static int	handle_shell_found_key(t_vars *shlvl_var, int *shlvl)
{
	if (!is_valid_shell_lvl(shlvl_var->value))
		return (EXIT_FAILURE);
	*shlvl = ft_atoi(shlvl_var->value);
	(*shlvl)++;
	free(shlvl_var->value);
	shlvl_var->value = ft_itoa(*shlvl);
	if (!shlvl_var->value)
		return (perror("Allocation failed"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_shell_lvl(t_shell *shell, char *key)
{
	char	*value;
	int		shlvl;
	t_vars	*shlvl_var;

	shlvl_var = find_key("SHLVL", shell->env);
	if (shlvl_var)
	{
		if (handle_shell_found_key(shlvl_var, &shlvl) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	else
	{
		key = ft_strdup("SHLVL");
		if (!key)
			return (perror("Allocation failed"), EXIT_FAILURE);
		value = ft_strdup("1");
		if (!value)
			return (free(key), perror("Allocation failed"), EXIT_FAILURE);
		shlvl_var = new_env(key, value);
		if (!shlvl_var)
			return (free(key), free(value),
				perror("Allocation failed"), EXIT_FAILURE);
		add_env(&(shell->env), shlvl_var);
	}
	return (EXIT_SUCCESS);
}

static int	init_clownshell(t_shell *shell, char **envp)
{
	shell->std_stdout = dup(STDOUT_FILENO);
	if (shell->std_stdout == -1)
		return (perror("dup"), EXIT_FAILURE);
	shell->std_stdin = dup(STDIN_FILENO);
	if (shell->std_stdin == -1)
		return (perror("dup"), close(shell->std_stdout), EXIT_FAILURE);
	if (init_env(envp, shell) != EXIT_SUCCESS)
		return (close(shell->std_stdout), close(shell->std_stdin), EXIT_FAILURE);
	shell->last_exit_code = 0;
	shell->exported_vars = NULL;
	shell->is_exit = 0;
	return (EXIT_SUCCESS);
}

static int	main_while(char *line, t_shell *shell)
{
	if (line && line[0] != '\0')
		add_history(line);
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_code = 130;
		g_signal_received = 0;
	}
	exec_commands(line, shell);
	if (g_signal_received == SIGINT)
		shell->last_exit_code = 130;
	free(line);
	if (dup2(shell->std_stdout, STDOUT_FILENO) == -1)
		return (perror("dup2"), rl_clear_history(),
			free_all_envs(&(shell->exported_vars)),
			free_all_envs(&(shell->env)), exit(EXIT_FAILURE), (1));
	if (dup2(shell->std_stdin, STDIN_FILENO) == -1)
		return (perror("dup2"), rl_clear_history(),
			free_all_envs(&(shell->exported_vars)),
			free_all_envs(&(shell->env)), exit(EXIT_FAILURE), (1));
	return (EXIT_SUCCESS);
}

static void	close_clownshell(t_shell *shell)
{
	//printf("exit\n");
	rl_clear_history();
	close(shell->std_stdin);
	close(shell->std_stdout);
	free_all_envs(&(shell->exported_vars));
	free_all_envs(&(shell->env));
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;
	int		exit_code;

	(void)argc;
	(void) argv;
	if (init_clownshell(&shell, envp)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (handle_shell_lvl(&shell, NULL) != EXIT_SUCCESS)
		return (free_all_envs(&(shell.env)), close(shell.std_stdin),
			close(shell.std_stdout), EXIT_FAILURE);
	while (!shell.is_exit)
	{
		init_signals(GLOBAL_MODE);
		g_signal_received = 0;
		if (isatty(fileno(stdin)))
			line = readline("🤡clownshell🤡$ ");
		else
		{
			char *temp;
			temp = get_next_line(fileno(stdin));
			if (temp == NULL)
				break;
			line = ft_strtrim(temp, "\n");
			free(temp);
		}
		if (line == NULL)
			break ;
		main_while(line, &shell);
	}
	exit_code = shell.last_exit_code;
	return (close_clownshell(&shell), exit_code);
}
