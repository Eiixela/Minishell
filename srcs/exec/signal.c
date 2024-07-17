/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:20:29 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/17 21:34:03 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status_child(t_line *line, int status)
{
	(void)line;
	(void)status;
 	if (line->nm_arg == 1)
		exit(g_ret);
	if (WIFEXITED(status))
		g_ret = WEXITSTATUS(status);
}

void	handle_exit_status_in_pipe(t_line *line)
{
	t_pipe	*current;
	
	current = line->pipe_head;
	while (current != NULL)
	{
		if (ft_strcmp(current->arg[0], "exit") == 0)
			g_ret = (unsigned char)ft_atoll(current->arg[1]);
		current = current->next;
	}
}
