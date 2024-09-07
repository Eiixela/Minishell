/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:36:52 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/07 18:54:17 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*gen_filename(int fn)
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

int	write_to_heredoc(int fd, const char *content)
{
	if (write(fd, content, ft_strlen(content)) == -1
		|| write(fd, "\n", 1) == -1)
	{
		ft_putstr_fd("error: write to heredoc failed\n", 2);
		return (0);
	}
	return (1);
}

int	open_and_dup_heredoc(char *fd_name)
{
	int	fd_file_heredoc;

	fd_file_heredoc = open(fd_name, O_RDONLY);
	if (fd_file_heredoc == -1)
	{
		perror("error on open for reading");
		return (0);
	}
	if (dup2(fd_file_heredoc, STDIN_FILENO) == -1)
	{
		close(fd_file_heredoc);
		perror("dup2 heredoc");
		return (0);
	}
	close(fd_file_heredoc);
	return (1);
}

int	read_heredoc_lines(int fd, char *delimiter, t_env *env)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		result = process_heredoc_line(fd, line, env);
		free(line);
		if (!result)
			return (0);
	}
	return (1);
}

int	handle_single_heredoc(char *delimiter, const char *temp_file, t_env *env)
{
	int		fd_file_heredoc;
	int		result;

	fd_file_heredoc = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_file_heredoc == -1)
	{
		ft_putstr_fd("error: cannot open file\n", 2);
		return (0);
	}
	result = read_heredoc_lines(fd_file_heredoc, delimiter, env);
	close(fd_file_heredoc);
	return (result);
}
