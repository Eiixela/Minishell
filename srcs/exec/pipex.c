/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/02 16:56:08 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd, t_line *line)
{
    pid_t pid;
    int saved_output;
    int builtin_result;
    int redir_result;

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
            redir_result = redirection_in_pipe(pipe, &saved_output, env);
            if (redir_result == 0)
                return (exit(EXIT_FAILURE), pid);
            else if (redir_result == 2)
                exit (pipe->ret_val);
        }
        if (parse_builtin(pipe))
        {
            builtin_result = execute_builtins(env, pipe, line);
            exit(builtin_result);  // Exit with the return value of the builtin
        }

        else
        {
            if (execute_cmd(env, pipe, line))
                exit(pipe->ret_val);
        }
    }
    return (pid);
}

int _call_childs(t_env *env, t_line *line)
{
    int     pipe_fd[2];
    int     input_fd;
    t_pipe  *current;
    pid_t   pid;
    pid_t   last_pid = 0;  // To keep track of the last process in the pipeline
    int     quit_message_printed = 0;
    int builtin_result;
    int status;

    current = line->pipe;
    input_fd = 0;
    builtin_result = parse_and_execute_solo_builtins(env, line);
    if (builtin_result == 2)
        return (1);
    if (builtin_result == 0)
        return (0);
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
        if (current->next == NULL)
            last_pid = pid;
        current = current->next;
    }
    pid_t wpid;
    sigend();
    while ((wpid = wait(&status)) > 0)
    {
        if (wpid == last_pid) {
            handle_exit_status_child(line, status, &quit_message_printed);
        }
    }
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
