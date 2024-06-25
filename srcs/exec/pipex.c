/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/25 12:48:25 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static int create_process(char **env, t_pipe *pipe_node, int input_fd, int output_fd)
{
    pid_t pid = fork();
    if (pid == -1)
        return (perror("fork"), 0);
    if (pid == 0)
    {
        if (input_fd != 0)
        {
            dup2(input_fd, 0);
            close(input_fd);
        }
        if (output_fd != 1)
        {
            dup2(output_fd, 1);
            close(output_fd);
        }
        if (!execute_cmd(env, pipe_node->arg))
            exit(EXIT_FAILURE);
    }
    return pid;
}

static int _call_childs(char **env, t_line line)
{
    int		pipe_fd[2];
    int		input_fd;
    int 	status;
    t_pipe	*current;
    pid_t	pid;

	input_fd = 0;
	current = line.pipe;
   while (current != NULL)
    {
        if (current->next != NULL)
        {
            if (pipe(pipe_fd) == -1)
                return (perror("pipe"), 0);
            pid = create_process(env, current, input_fd, pipe_fd[1]);
            close(pipe_fd[1]);
            if (input_fd != 0)
                close(input_fd);
            input_fd = pipe_fd[0];
        }
        else
        {
            pid = create_process(env, current, input_fd, 1);
            if (input_fd != 0)
                close(input_fd);
        }
        current = current->next;
    }
    while (wait(&status) > 0)
        ;
    return (1);
}

int	pipex(char **env, t_line line)
{
	if(!_call_childs(env, line))
		return (0);
	return (1);
} 