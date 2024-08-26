/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:29:47 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/26 15:18:54 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*gen_filename(int fn)
{
	char	*strfn;
	char	*filename;

	strfn = ft_itoa(fn);
	if (!strfn)
		return (print_error(errno, "minishell: heredoc"));
	filename = ft_strjoin("/tmp/tmp_", strfn);
	free(strfn);
	if (!filename)
		return (print_error(errno, "minishell: heredoc"));
	return (filename);
}

// Function to handle a single heredoc
static int handle_single_heredoc(char *delimiter, const char *temp_file)
{
    int fd_file_heredoc;
    char *line_heredoc;

    fd_file_heredoc = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_file_heredoc == -1)
        return (fprintf(stderr, "error on open here \n"), 0);
    while (1)
    {
        line_heredoc = readline("> ");
        if (!line_heredoc || ft_strcmp(line_heredoc, delimiter) == 0)
        {
            free(line_heredoc);
            break;
        }
        write(fd_file_heredoc, line_heredoc, ft_strlen(line_heredoc));
        write(fd_file_heredoc, "\n", 1);
        free(line_heredoc);
    }
    close(fd_file_heredoc);
    return (1);
}

// Modified function to handle multiple heredocs
static int redir_heredoc(t_pipe *pipe)
{
    char *temp_file;
    int heredoc_count = 0;
    int fd_file_heredoc;
    t_redir *current_redir = pipe->redir;

    while (current_redir && current_redir->type == HEREDOC)
    {
        temp_file = gen_filename(heredoc_count);
        if (!handle_single_heredoc(current_redir->fd, temp_file))
            return 0;
        current_redir = current_redir->next;
        heredoc_count++;
    }
    fd_file_heredoc = open(temp_file, O_RDONLY);
    if (fd_file_heredoc == -1)
        return (perror("error on open for reading"), 0);
    if (dup2(fd_file_heredoc, STDIN_FILENO) == -1)
        return (perror("dup2 heredoc"), 0);
    close(fd_file_heredoc);
    for (int i = 0; i < heredoc_count; i++)
        unlink(temp_file);
    return (1);
}

static t_redir *redirection_append_and_out(t_redir *current_redir)
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
        perror("opening file");
        return (NULL);
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
    return 1;
}

static int last_redir(t_redir *last_out_redir)
{
    int flags;
    int fd;

    fprintf(stderr, " here!\n");
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
    return 1;
}

int redirection_in_pipe(t_pipe *pipe, int *saved_output)
{
    t_redir *current_redir;
    t_redir *last_out_redir = NULL;
    int heredoc_processed = 0;

    *saved_output = dup(STDOUT_FILENO);
    if (*saved_output == -1)
        return (perror("dup"), 0);
    current_redir = pipe->redir;
    while (current_redir != NULL)
    {
        if (current_redir->type == HEREDOC && !heredoc_processed)
        {
            if (redir_heredoc(pipe) == 0)
                return (fprintf(stderr, "here heredoc\n"), 0);
            heredoc_processed = 1;
        }
        else if (current_redir->type == OUT_REDIR || current_redir->type == APPEND)
            last_out_redir = redirection_append_and_out(current_redir);
        else if (current_redir->type == IN_REDIR)
            if (!redirection_in(current_redir))
                return 0;
        current_redir = current_redir->next;
    }
    if (last_out_redir)
        if (!last_redir(last_out_redir))
            return (0);
    return (1);
}
