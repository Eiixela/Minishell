/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:43:13 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/14 15:20:11 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_on_open(t_line line)
{
	if (ft_strlen(line.pipe->redir->filename) > 0)
		ft_putstr_fd("minishell: ", 2);
	else
		ft_putstr_fd("minishell: : ", 2);
	perror(line.pipe->redir->filename);
	if (line.pipe->redir->fd_in != -1)
		close (line.pipe->redir->fd_in != -1);
	if (line.pipe->redir->fd_out != -1)
		close (line.pipe->redir->fd_out != -1);
	line.pipe->redir->fd_out = -1;
	//free line.pipe->redir->filename ??
	line.pipe->redir->fd_in = -1;
	line.return_val = 1;
}

static void _action_command_not_found(t_line line)
{
	ft_putstr_fd("minishell: : command not found\n", 2);
	line.return_val = 127;
}

int	io_open_fds(t_line line)
{
	int	heredoc;

	if (_init(line))
		return (0);
	while (line.pipe)
	{
		if (!action_on_files(line.pipe->redir, heredoc))
			return (0);
		line.pipe = line.pipe->next;
	}
	if (heredoc)
	{
		if (line.pipe->redir != -1)
			close(line.pipe->redir->fd_in);
		line.pipe->redir->fd_in = open(line.pipe->redir->filename, O_RDONLY);
		if (line.pipe->redir->fd_in == -1)
			return (error_on_open(line), 0);
	}
	return (1);
	
}

//TODO fd out and in in struct and what to free ??
int	open_fds(char **env, t_line line, size_t cmdnbr)
{
	if (io_open_fds(line))
		return (1);
	if (!(line.pipe->arg) || !(line.pipe->arg) || ((line.pipe->arg[0]) && !(line.pipe->arg[0][0])))
		_action_command_not_found(line);
	if (line.pipe->redir->type == OUT_REDIR)
	{
		//close (line.pipe->redir->fd_out); ++ free line.pipe->redir->type and filename ?? 
		line.pipe->redir->type == NULL;
	}
	if (line.pipe->redir->type == IN_REDIR)
	{
		//close (line.pipe->redir->fd_out); ++ free line.pipe->redir->type and filename ??
		line.pipe->redir->type == NULL;
	}
	return (0);
}