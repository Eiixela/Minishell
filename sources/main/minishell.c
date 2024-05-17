/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/17 18:12:04 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	main (int ac, char **av, char **env)
{
	char	*line;

	(void)ac;
	(void)av;
	(void)env;
	line = NULL;
	while (1)
	{
		line = readline("minishell >> ");
		if (line && *line)
		{
			pwd();
			add_history(line);
			free(line);
			line = NULL;			
		}
	}
	rl_clear_history();
	return (0);
}