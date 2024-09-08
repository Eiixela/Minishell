/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:03:03 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/08 19:03:10 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status_child(t_line *line, int status,
		int quit_message_printed)
{
	if (WIFEXITED(status))
	{
		line->pipe->ret_val = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		line->pipe->ret_val = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT && !(quit_message_printed))
		{
			printf("Quit (core dumped)\n");
			quit_message_printed = 1;
		}
	}
}
