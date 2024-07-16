/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:20:29 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/16 14:55:45 by aljulien         ###   ########.fr       */
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
