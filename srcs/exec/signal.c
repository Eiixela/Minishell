/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:26:33 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 18:27:25 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nonull(void)
{
	return (1);
}

void	sighandler(int sig)
{
	g_ret = sig;
	if (g_ret == SIGINT)
		rl_done = g_ret;
	return ;
}

void	sigend(void)
{
	g_ret = -1;
}

void	siglisten(void)
{
	rl_event_hook = get_nonull;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
}

char	*send_eof(char *line)
{
	if (!line)
		return (NULL);
	return (line);
}
