/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/25 11:07:20 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	first_child(char **env, t_line line, pid_t pid, int pipe_fd[2])
{
	pid = fork();
	if (pid == 0)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		if (!execute_cmd(env, line.pipe->arg))
			return (0);
	}
	return (1);
}

static int	last_child(char **env, t_line line, pid_t pid, int pipe_fd[2])
{
	pid = fork();
	//pid_error
	if (pid == 0)
	{
 		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
			if (!execute_cmd(env, line.pipe->arg))
				return (0);
	}
	return (1);
}

static int	_call_childs(char **env, t_line line)
{
	pid_t	pid_first_child;
	pid_t	pid_last_child;
	int		status;
	int		pipe_fd[2];
	
	pid_first_child = 0;
	pid_last_child = 0;
	if (line.pipe->next != NULL)
	{
		if(pipe(pipe_fd) == -1)
			return (perror("pipe"), 0);
		if (!first_child(env, line, pid_first_child, pipe_fd))
			return (0);
	}
	if (line.pipe->next != NULL)
		line.pipe = line.pipe->next;
	if (!last_child(env, line, pid_last_child, pipe_fd))
		return (0);
	waitpid(pid_first_child, &status, 0);
	waitpid(pid_last_child, &status, 0);
	return (1);
}

int	pipex(char **env, t_line line)
{
	if(!_call_childs(env, line))
		return (0);
	return (1);
} 