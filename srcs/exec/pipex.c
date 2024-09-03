/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/03 09:21:07 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd, t_line *line)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 0);
    if (pid == 0)
    {
        signal(SIGQUIT, SIG_DFL);
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
        if (pipe->redir != NULL)
        {
            int saved_output;
            int redir_result = redirection_in_pipe(pipe, &saved_output, env);
            if (redir_result == 0)
                exit(EXIT_FAILURE);
            else if (redir_result == 2)
                exit(pipe->ret_val);
        }
        execute_cmd(env, pipe, line);
        exit(pipe->ret_val);
    }
    return (pid);
}



int _call_childs(t_env *env, t_line *line)
{
    int pipe_fd[2], input_fd = 0, status;
    t_pipe *current = line->pipe;
    pid_t pid, last_pid = 0;
    int cat_count = 0;

    (void)last_pid;
    // Count initial cat commands
    while (current && strcmp(current->arg[0], "cat") == 0 && current->arg[1] == NULL && !current->redir)
    {
        cat_count++;
        current = current->next;
    }
    current = line->pipe;

    while (current != NULL)
    {
        if (current->next != NULL)
        {
            if (pipe(pipe_fd) == -1)
                return (perror("pipe"), 0);

            if (cat_count > 0)
            {
                pid = fork();
                if (pid == 0)
                {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[1]);
                    char buf[1024];
                    if (read(STDIN_FILENO, buf, sizeof(buf)) > 0)
                        write(STDOUT_FILENO, buf, strlen(buf));
                    exit(0);
                }
                cat_count--;
            }
            else
            {
                pid = create_process(env, current, input_fd, pipe_fd[1], line);
            }
            close(pipe_fd[1]);
            if (input_fd != 0)
                close(input_fd);
            input_fd = pipe_fd[0];
        }
        else
        {
            pid = create_process(env, current, input_fd, 1, line);
            if (input_fd != 0)
                close(input_fd);
        }
        last_pid = pid;
        current = current->next;
    }

    while (wait(&status) > 0);
    
    // Handle remaining cat inputs
    while (cat_count > 0)
    {
        char buf[2];
        read(STDIN_FILENO, buf, 1);
        cat_count--;
    }

    return (1);
}

int	pipex(t_env *env, t_line *line, int *status)
{
	line->pipe->ret_val = *status;
	if (!_call_childs(env, line))
		return (0);
	return (1);
}
