/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 redirections.c										:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: aljulien <aljulien@student.42.fr>			+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/13 12:29:47 by aljulien		   #+#	  #+#			  */
/*	 Updated: 2024/09/03 11:29:32 by aljulien		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*redirection_append_and_out(t_redir *current_redir)
{
	int	fd;
	int	flags;

	if (current_redir->type == APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(current_redir->fd, flags, 0644);
	if (fd == -1)
	{
		perror("opening	file");
		return (NULL);
	}
	close(fd);
	return (current_redir);
}

static int	redirection_in(t_redir *current_redir)
{
	int	fd;

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

static int	last_redir(t_redir *last_out_redir)
{
	int	flags;
	int	fd;

	if (last_out_redir->type == APPEND)
		flags = O_WRONLY | O_APPEND;
	else
		flags = O_WRONLY | O_TRUNC;
	fd = open(last_out_redir->fd, flags, 0777);
	if (fd == -1)
		return (perror("opening	last output	file"), 0);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2 output"), 0);
	close(fd);
	return (1);
}

int	redirection_in_pipe(t_pipe *pipe, int *saved_output, t_env *env)
{
	t_redir	*current_redir;
	t_redir	*last_out_redir;
	int		heredoc_processed;
	int		heredoc_result;

	last_out_redir = NULL;
	heredoc_processed = 0;
	*saved_output = dup(STDOUT_FILENO);
	if (*saved_output == -1)
		return (perror("dup"), 0);
	current_redir = pipe->redir;
	while (current_redir != NULL)
	{
		if (current_redir->type == HEREDOC && !heredoc_processed)
		{
			heredoc_result = redir_heredoc(pipe, env);
			if (heredoc_result == 0)
				return (ft_putstr_fd("here heredoc\n", 2), 0);
			else if (heredoc_result == 1 && pipe->redir->next == NULL
				&& pipe->arg == NULL)
				return (2);
			heredoc_processed = 1;
		}
		else if (current_redir->type == OUT_REDIR
			|| current_redir->type == APPEND)
			last_out_redir = redirection_append_and_out(current_redir);
		else if (current_redir->type == IN_REDIR)
			if (!redirection_in(current_redir))
				return (0);
		current_redir = current_redir->next;
	}
	if (last_out_redir)
		if (!last_redir(last_out_redir))
			return (0);
	return (1);
}
