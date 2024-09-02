/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/02 17:15:26 by aljulien         ###   ########.fr       */
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
        if (strcmp(pipe->arg[0], "cat") == 0 && pipe->arg[1] == NULL && isatty(STDIN_FILENO))
        {
            char buf[1024];
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n > 0)
                write(STDOUT_FILENO, buf, n);
            exit(0);
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

	(void)last_pid;
    while (current != NULL)
    {
        if (current->next != NULL)
        {
            if (pipe(pipe_fd) == -1)
                return (perror("pipe"), 0);

            pid = create_process(env, current, input_fd, pipe_fd[1], line);
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
    return (1);
}

/*int _call_childs(t_env *env, t_line *line)
{
    int     pipe_fd[2];
    int     input_fd;
    t_pipe  *current;
    pid_t   pid;
    pid_t   last_pid = 0;
    int     quit_message_printed = 0;
    int     builtin_result;
    int     status;
    int     cat_count = 0;

    current = line->pipe;
    input_fd = 0;

    builtin_result = parse_and_execute_solo_builtins(env, line);
    if (builtin_result == 2)
        return (1);
    if (builtin_result == 0)
        return (0);

    // Count initial cat commands
    while (current && strcmp(current->arg[0], "cat") == 0 && current->arg[1] == NULL)
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
                    ssize_t n;
                    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
                    {
                        write(STDOUT_FILENO, buf, n);
                    }
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
        if (current->next == NULL)
            last_pid = pid;
        current = current->next;
    }
    pid_t wpid;
    sigend();
    while ((wpid = wait(&status)) > 0) 
    {
        if (wpid == last_pid)
            handle_exit_status_child(line, status, &quit_message_printed);
    }
    return (1);
}*/

int	pipex(t_env *env, t_line *line, int *status)
{
	line->pipe->ret_val = *status;
	if (!_call_childs(env, line))
		return (0);
	return (1);
}
