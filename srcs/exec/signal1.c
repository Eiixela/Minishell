/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:03:03 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/07 18:03:27 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status_child(t_line *line, int status,
		int quit_message_printed, int *cat_count)
{
	(void)cat_count;
	if (WIFEXITED(status))
	{
		cat_count = 0;
		line->pipe->ret_val = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		line->pipe->ret_val = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT && !(quit_message_printed))
		{
			cat_count = 0;
			printf("Quit (core dumped)\n");
			quit_message_printed = 1;
		}
	}
}
