/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:29:47 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/13 12:30:54 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    {
        ft_putstr_fd("minishell: ", STDOUT_FILENO);
        ft_putstr_fd(current_redir->fd, STDOUT_FILENO);
		return (perror(" "), 0);
    }
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