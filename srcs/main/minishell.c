/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 18:04:01 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	g_ret = 0;

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_line	line;

	(void)av;
	(void)ac;
	line.env = env;
	str = NULL;
	while (1)
	{
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
