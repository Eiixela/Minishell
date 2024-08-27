/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/27 14:23:53 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_process(t_env *env, t_pipe *pipe, int input_fd, int output_fd, t_line *line)
{
    pid_t pid;
    int saved_output;

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 0);
    if (pid == 0)
    {
        // Reset SIGQUIT handler to default for child processes
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
            if (!redirection_in_pipe(pipe, &saved_output))
                return (exit(EXIT_FAILURE), pid);
        }
        if (parse_builtin(pipe))
        {
            if (!execute_builtins(env, pipe, line))
                exit(pipe->ret_val);  // Exit with the return value of the builtin
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
    int     quit_message_printed = 0;  // Flag to track if we've printed the quit message

    current = line->pipe;
    input_fd = 0;
    
    int builtin_result = parse_and_execute_solo_builtins(env, line);
    if (builtin_result == 2)  // Builtin was executed
        return (1);
    if (builtin_result == 0)  // Error occurred
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
        current = current->next;
    }
    sigend();
    pid_t wpid;
    int status;
    while ((wpid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT && !quit_message_printed)
            quit_message_printed = 0;
        handle_exit_status_child(line, status, &quit_message_printed);
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
