/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/26 17:38:07 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	main (int ac, char **av, char **env)
{
	char	*str;
	t_line	line;
	
	(void)av;
	(void)ac;
	(void)env;
	str = NULL;
	while (1)
	{
		str = readline("minishell >> ");
		if (str && *str)
		{
			add_history(str);
			if (big_parse(&line, &str) == true)
			{
				printf("\n");
				if (!pipex(env, line))
					perror("execve");
				printf("\n");
			}	
		}
	}
	clear_history();
	return (0);
}
