/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:05:33 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/14 12:35:51 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//TODO free struct but which one ? where does Sam malloc and free ?
/* void	free_line(t_line line)
{
	line.pipe = line.pipe_head;
	
} */

//TODO should i open the file beforehand and stock the fds into the structure or just leave it as be ?
void	exit_child(char **env, t_line line, size_t cmdnbr)
{
	(void)env;
	(void)cmdnbr;
/* 	if (line.pipe->redir->type == OUT_REDIR)
		close (fd_outfile);
	if (line.pipe->redir->type == IN_REDIR)
		close (fd_infile); */
	//free_line(line);
	exit (line.return_val);
}