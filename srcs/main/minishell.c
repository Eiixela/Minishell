/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:39:24 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 16:39:27 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

static void	process_input(char **str, t_line *line, t_env *env, int *status)
{
	char	*cpy;

	cpy = *str;
	add_history(*str);
	*str = big_parse(line, str, *status);
	if (*str)
	{
		if (*str && *str != cpy)
		{
			free(*str - line->skipped_char);
			*str = NULL;
		}
		line->pipe->ret_val = *status;
		if (!pipex(env, line, status, *str))
			perror("execve");
		line->exit_status = line->pipe->ret_val;
		cleanup(line);
	}
	else
		cleanup(line);
}

int	main(int ac, char **av, char **envp)
{
	char			*str;
	static t_line	line;
	t_env			*env;
	int				status;

	(void)av;
	initialize(&line, &status);
	if (ac != 1)
	{
		print_error(errno, "minishell: too many arguments");
		return (0);
	}
	if (!setup_environment(&env, envp, &line))
		return (0);
	while (1)
	{
		sigend();
		str = readline("aljulien@z3r8p5:~/goinfre/minishell$ ");
		if (!str)
			handle_exit(&line, env);
		line.exit_status = status;
		if (str && *str && g_ret != SIGINT)
			process_input(&str, &line, env, &status);
		update_status(&status, &line);
	}
}
