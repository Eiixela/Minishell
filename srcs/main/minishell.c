/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:45:51 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/11 18:07:58 by aljulien         ###   ########.fr       */
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
			big_parse(&line, &str);
			pipex(env, line);
		}
	}
	clear_history();
	return (0);
}
