/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/13 15:19:30 by aljulien         ###   ########.fr       */
=======
/*   Updated: 2024/07/30 18:04:01 by saperrie         ###   ########.fr       */
>>>>>>> a81b1ccc477598d27c263152b8076dd68edcd30a
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

	(void)av;
	(void)ac;
	line.env = env;
	str = NULL;
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
			if (big_parse(&line, &str) == true)
			{
				if (!pipex(env, &line))
					perror("execve");
			}
		}
	}
	clear_history();
	return (0);
}
