/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/23 14:17:38 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret = 0;

static int init_env(t_env **env, char **envp)
{
	/* if (!isatty(0) || isatty(1))
		return (print_error(errno, "minishell"), 0); */
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
	(void)ac;
	str = NULL;
	status = 0;
	if (ac != 1)
		return(print_error(errno, "minishell: too many arguments"), 1);
	if (!init_env(&env, envp))
		return (1);
	while (1)
	{
		sigend();
		str = readline("aljulien@z3r8p5:~/goinfre/minishell$ ");
		if (str && *str)
		{
			add_history(str);
			if (big_parse(&line, &str, env, &status) == true)
			{
				if (!pipex(env, &line, &status))
					perror("execve");
				if (g_ret == SIGINT)
					status = 128 + g_ret;
			}
		}
	}
	clear_history();
	exit(status);
	return (0);
}
