/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/27 15:49:47 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

static void	init_line(t_line *line)
{
	line->argv = NULL;
	line->argv_head = NULL;
	line->pipe = NULL;
	line->pipe_head = NULL;
}

static int init_env(t_env **env, char **envp)
{
	/*if (!isatty(0) || isatty(1))
		return (print_error(errno, "minishell"), 0);*/
	*env = NULL;
	create_env(envp, env);
	siglisten();
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*str;
	t_line	line;
	t_env	*env;
	int		status;

	(void)av;
	str = NULL;
	status = 0;
	if (ac != 1)
		return (print_error(errno, "minishell: too many arguments"), 1);
	if (!init_env(&env, envp))
		return (1);
	line.env = env;
	init_line(&line);
	while (1)
	{
		fprintf(stderr, "%i\n", status);
		sigend();
		str = readline("aljulien@z3r8p5:~/goinfre/minishell$ ");
		if (!str)
			return (cleanup(&line), free_env(env), 0);
		if (str && *str && g_ret != SIGINT)
		{
			add_history(str);
			if (big_parse(&line, &str) == true)
			{
				line.pipe->ret_val = status;
				if (!pipex(env, &line, &status))
					perror("execve");
			}
		}
		if (g_ret == SIGINT)
			status = 128 + g_ret;
		else
			status = line.pipe->ret_val;
		if (str)
			free(str);
		cleanup(&line);
	}
	free_env(env);
	clear_history();
	exit(status);
	return (0);
}
