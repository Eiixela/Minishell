/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:48:55 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/05 15:43:36 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	redir_heredoc(t_pipe *pipe)
{
	int     fd_file_heredoc;
    char    *path_to_heredoc_file;
    char    *line_heredoc;
    int     fd;
    
    fd = 0;
    path_to_heredoc_file = "/tmp/heredoc";
    fd_file_heredoc = open(path_to_heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_file_heredoc == -1)
		return (perror("error on open"), 0);
    if (dup2(fd, STDIN_FILENO) == -1)
        return (perror("dup2 heredoc here!"), 0);
    while (1)
    {
        line_heredoc = readline("> ");
        if (ft_strcmp(line_heredoc, pipe->redir->fd) == 0)
        {
            close(fd_file_heredoc);
            free(line_heredoc);
            close(fd);
            return (0);
        }
        write(fd_file_heredoc, line_heredoc, ft_strlen(line_heredoc));
        write(fd_file_heredoc, "\n", 1);
        free(line_heredoc);
    }
	return (1);
} */

static int redir_heredoc(t_pipe *pipe)
{
    int fd_file_heredoc;
    char *path_to_heredoc_file = "/tmp/heredoc";
    char *line_heredoc;

    fd_file_heredoc = open(path_to_heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_file_heredoc == -1)
        return (perror("error on open"), 0);
    while (1)
    {
        line_heredoc = readline("> ");
        if (!line_heredoc || ft_strcmp(line_heredoc, pipe->redir->fd) == 0)
        {
            free(line_heredoc);
            break;
        }
        write(fd_file_heredoc, line_heredoc, ft_strlen(line_heredoc));
        write(fd_file_heredoc, "\n", 1);
        free(line_heredoc);
    }
    close(fd_file_heredoc);
    fd_file_heredoc = open(path_to_heredoc_file, O_RDONLY);
    if (fd_file_heredoc == -1)
        return (perror("error on open for reading"), 0);
    if (dup2(fd_file_heredoc, STDIN_FILENO) == -1)
        return (perror("dup2 heredoc"), 0);
    close(fd_file_heredoc);
    return (1);
}

static t_redir  *redirection_append_and_out(t_redir *current_redir)
{
    int fd;
    int flags;
    
    if (current_redir->type == APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(current_redir->fd, flags, 0644);
    if (fd == -1)
    {
        current_redir = NULL;
        perror("opening file");
    }
    close(fd);
    return (current_redir);
}

static int redirection_in(t_redir *current_redir)
{
    int fd;
    
    fd = open(current_redir->fd, O_RDONLY);
	if (fd == -1)
		return (perror("opening input file"), 0);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2 input"), 0);
    close(fd);
	return (1);
}

static int last_redir(t_redir *last_out_redir)
{
	int flags;
	int fd;

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
	return (1);
}

/* int redirection_in_pipe(t_pipe *pipe, int *saved_output)
{
    t_redir *current_redir;
    t_redir *last_out_redir = NULL;

    *saved_output = dup(STDOUT_FILENO);
    if (*saved_output == -1)
        return (perror("dup"), 0);
    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == HEREDOC)
            if (redir_heredoc(pipe) == -1)
                return (perror("heredoc"), 0);
        if (current_redir->type == OUT_REDIR || current_redir->type == APPEND)
            last_out_redir = redirection_append_and_out(current_redir);
        if (current_redir->type == IN_REDIR)
            if (!redirection_in(current_redir))
				return (0);
        current_redir = current_redir->next;
	}
    if (last_out_redir)
        if (!last_redir(last_out_redir))
			return (0);
    return (1);
} */

int redirection_in_pipe(t_pipe *pipe, int *saved_output)
{
    t_redir *current_redir;
    t_redir *last_out_redir = NULL;

    *saved_output = dup(STDOUT_FILENO);
    if (*saved_output == -1)
        return (perror("dup"), 0);

    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == HEREDOC)
        {
            if (redir_heredoc(pipe) == 0)
                return (perror("heredoc"), 0);
        }
        else if (current_redir->type == OUT_REDIR || current_redir->type == APPEND)
        {
            last_out_redir = redirection_append_and_out(current_redir);
        }
        else if (current_redir->type == IN_REDIR)
        {
            if (!redirection_in(current_redir))
                return (0);
        }
        current_redir = current_redir->next;
    }
    if (last_out_redir)
        if (!last_redir(last_out_redir))
            return (0);

    return (1);
}

/* static int	create_process(char **env, t_pipe *pipe, int input_fd, int output_fd)
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
			if (!redirection_in_pipe(pipe, &saved_output))
                return (exit(EXIT_FAILURE), pid);
	 	if (execute_cmd(env, pipe))
			exit(pipe->ret_val);
	}
	return (pid);
} */


int create_process(char **env, t_pipe *pipe, int input_fd, int output_fd)
{
    pid_t pid;
    int saved_output;

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
            if (!redirection_in_pipe(pipe, &saved_output))
                return (exit(EXIT_FAILURE), pid);

        if (parse_builtin(pipe))
        {
            if (!parse_and_execute_solo_builtins(pipe))
                exit(EXIT_SUCCESS);
        }
        else
        {
            if (execute_cmd(env, pipe))
                exit(pipe->ret_val);
        }
    }
    return (pid);
}

int	_call_childs(char **env, t_line *line)
{
	int		pipe_fd[2];
	int		input_fd;
	int		status;
	t_pipe	*current;
	pid_t	pid;
    
	(void)pid;
	current = line->pipe;
	input_fd = 0;
 	if (parse_and_execute_solo_builtins(line->pipe) == 0)
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
