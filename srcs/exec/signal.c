/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:20:29 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/27 15:23:52 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_exit_status_child(t_line *line, int status, int *quit_message_printed)
{   
    if (WIFEXITED(status))
    {
        line->pipe->ret_val = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGQUIT && !(*quit_message_printed))
        {
            printf("Quit (core dumped)\n");
            *quit_message_printed = 1;
        }
        line->pipe->ret_val = 128 + WTERMSIG(status);
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

void siglisten(void)
{
    rl_event_hook = get_nonull;
    signal(SIGINT, sighandler);
    signal(SIGQUIT, SIG_IGN);  // Changed from SIG_IGN to sigquit_handler
}

char	*send_eof(char *line)
{
	if (!line)
		return (NULL);
	return (line);
}
