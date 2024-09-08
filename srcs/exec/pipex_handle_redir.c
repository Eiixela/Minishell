/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_handle_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:59:23 by ubuntu            #+#    #+#             */
/*   Updated: 2024/09/08 13:50:46 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(char *filename, t_line *line, t_env *env)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_error_message("minishell: ", filename,
			": No such file or directory\n");
		cleanup(line);
		free_env(env);
		exit(1);
	}
	return (fd);
}

int	handle_output_redirection(char *filename, t_redir *redir,
				t_line *line, t_env *env)
{
	int	flags;
	int	fd;

	if (redir->type == APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		print_error_message("minishell: ", filename,
			": Failed to open output file\n");
		cleanup(line);
		free_env(env);
		exit(1);
	}
	return (fd);
}

int	handle_heredoc(const char *filename, t_line *line, t_env *env)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc");
		cleanup(line);
		free_env(env);
		exit(1);
	}
	return (fd);
}

int	handle_redirection(t_pipe *pipe)
{
	int	saved_output;
	int	redir_result;

	redir_result = redirection_in_pipe(pipe, &saved_output);
	if (redir_result == 0)
		return (0);
	if (redir_result == 2)
		return (2);
	return (1);
}
