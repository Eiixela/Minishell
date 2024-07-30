/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 14:54:31 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	redir_heredoc(t_pipe *pipe)
{
	int     fd_file_heredoc;
    char    *path_to_heredoc_file;
    char    *line_heredoc;

    path_to_heredoc_file = "/tmp/heredoc";
    fd_file_heredoc = open(path_to_heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_file_heredoc == -1)
        return (0);
    while (1)
    {
        line_heredoc = readline("> ");
        if (ft_strcmp(line_heredoc, pipe->redir->fd) == 0)
            return (1);
    }
	return (0);
}

static int redirection_in_pipe(t_pipe *pipe, int *saved_output)
{
    t_redir *current_redir;
    t_redir *last_out_redir = NULL;
    int fd;
    int flags;

    *saved_output = dup(STDOUT_FILENO);
    if (*saved_output == -1)
        return (perror("dup"), 0);
    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == OUT_REDIR || current_redir->type == APPEND)
        {
            if (current_redir->type == APPEND)
                flags = O_WRONLY | O_CREAT | O_APPEND;
            else
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            fd = open(current_redir->fd, flags, 0644);
            if (fd == -1)
                return (perror("opening file"), 0);
            close(fd);
            last_out_redir = current_redir;
		}
        else if (current_redir->type == IN_REDIR)
        {
            fd = open(current_redir->fd, O_RDONLY);
            if (fd == -1)
                return (perror("opening input file"), 0);
            if (dup2(fd, STDIN_FILENO) == -1)
                return (perror("dup2 input"), 0);
            close(fd);
        }
        else if (current_redir->type == HEREDOC)
        {
            if (!redir_heredoc(pipe))
			{
				fprintf(stderr, "cc");
                return (perror("heredoc"), 0);
			}
			else
                return (1);
        }
        current_redir = current_redir->next;
	}
    if (last_out_redir)
    {
        if (last_out_redir->type == APPEND)
            flags = O_WRONLY | O_APPEND;
        else
            flags = O_WRONLY | O_TRUNC;
        fd = open(last_out_redir->fd, flags, 0777);
        if (fd == -1)
            return (perror("opening last output file"), 0);
        if (dup2(fd, STDOUT_FILENO) == -1)
            return (perror("dup2 output"), 0);
        close(fd);
	}
    return (1);
}

//TODO make struct env for ome builtins
static int parse_and_execute_solo_buitlins(t_line *line)
{
    int saved_output = -1;
    if (parse_builtin(line->pipe) == 1 && line->pipe->arg)
    {

        if (line->pipe->redir != NULL)
        {
            if (!redirection_in_pipe(line->pipe, &saved_output))
                return (0);
        }
        if (!ft_strcmp(line->pipe->arg[0], "echo"))
            ft_echo(line->pipe->arg); // Directly call the echo function
        else if (!ft_strcmp(line->pipe->arg[0], "cd"))
            ; // Implement cd logic
        else if (!ft_strcmp(line->pipe->arg[0], "pwd"))
            ft_pwd(line->pipe->arg);
        else if (!ft_strcmp(line->pipe->arg[0], "export"))
            printf("export\n");
        else if (!ft_strcmp(line->pipe->arg[0], "unset"))
            printf("unset\n");
        else if (!ft_strcmp(line->pipe->arg[0], "env"))
            ; // Implement env logic
        else if (!ft_strcmp(line->pipe->arg[0], "exit"))
            ft_exit(line->pipe);
        else
    		return 1; // Command not recognized
        if (saved_output != -1)
        {
            if (dup2(saved_output, STDOUT_FILENO) == -1)
                perror("dup2");
            close(saved_output);
        }
        return (1); // Successfully executed the command
    }
    return (1); // No built-in command found
}


static int	create_process(char **env, t_pipe *pipe, int input_fd, int output_fd)
{
	pid_t	pid;
	int		saved_output;

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
		if (pipe->redir != NULL)
		{
			if (!redirection_in_pipe(pipe, &saved_output))
				exit(EXIT_FAILURE);
        }
	 	if (execute_cmd(env, pipe))
			exit(pipe->ret_val);
		exit(pipe->ret_val);
	}
	return (pid);
}


static	int	_call_childs(char **env, t_line *line)
{
	int		pipe_fd[2];
	int		input_fd;
	int		status;
	t_pipe	*current;
	pid_t	pid;

	(void)pid;
	current = line->pipe;
	input_fd = 0;

 	if (parse_and_execute_solo_buitlins(line) == 0)
		return (1);
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
		handle_exit_status_child(line, status);
	return (1);
}

int	pipex(char **env, t_line *line)
{
	if (!_call_childs(env, line))		
		return (0);
	return (1);
}
