/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/25 15:20:45 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static int	redirection_in_pipe(t_pipe *pipe)
{
	t_redir *current_redir;
    int     fd;

    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == OUT_REDIR)
        {
            fd = open(current_redir->fd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd == -1)
                return (perror("opening file"), 0);
            if (dup2(fd, STDOUT_FILENO) == -1)
                return (perror("dup2"), 0);
            close(fd);
        }
        else if (current_redir->type == IN_REDIR)
        {
            fd = open(current_redir->fd, O_RDONLY, 0777);
            if (fd == -1)
                return (perror("opening file"), 0);
            if (dup2(fd, STDIN_FILENO) == -1)
                return (perror("dup2"), 0);
            close(fd);
        }
        current_redir = current_redir->next;
    }
    return (1);
}

static int create_process(char **env, t_pipe *pipe_node, int input_fd, int output_fd)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 0);
    if (pid == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            if (dup2(input_fd, STDIN_FILENO) == -1)
                return (perror("dup2 input_fd"), 0);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            if (dup2(output_fd, STDOUT_FILENO) == -1)
                return (perror("dup2 output_fd"), 0);
            close(output_fd);
        }
        if (pipe_node->redir != NULL)
        {
            if (!redirection_in_pipe(pipe_node))
                exit(EXIT_FAILURE);
        }
        if (!execute_cmd(env, pipe_node->arg))
            exit(EXIT_FAILURE);
    }
    return (pid);
}

static int _call_childs(char **env, t_line line)
{
    int		pipe_fd[2];
    int		input_fd;
    int		status;
    t_pipe	*current;
    pid_t	pid;

	current = line.pipe;
	input_fd = 0;
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

int pipex(char **env, t_line line)
{
    if (!_call_childs(env, line))
        return 0;
    return 1;
}