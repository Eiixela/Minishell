/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/05 15:55:48 by aljulien         ###   ########.fr       */
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
	(void)env;
	str = NULL;
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
