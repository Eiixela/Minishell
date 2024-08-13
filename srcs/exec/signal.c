/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:20:29 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/13 15:35:57 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status_child(t_line *line, int status)
{
	(void)line;
	(void)status;
 	if (line->pipe->arg)
	{
		if (line->nm_arg == 1 && !ft_strcmp(line->pipe->arg[0], "exit"))
			exit(line->pipe->ret_val);
		if (WIFEXITED(status))
			line->pipe->ret_val = WEXITSTATUS(status);
	}
}

/* void	handle_exit_status_in_pipe(t_line *line)
{
	t_pipe	*current;
	
	current = line->pipe_head;
	while (current != NULL)
	{
		if (ft_strcmp(current->arg[0], "exit") == 0)
		{	
			line->pipe->ret_val = (unsigned char)ft_atoll(current->arg[1]);
		}
		current = current->next;
	}
} */

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
	signal(SIGQUIT, SIG_IGN);
}

char	*send_eof(char *line)
{
	if (!line)
		return (NULL);
	return (line);
}
