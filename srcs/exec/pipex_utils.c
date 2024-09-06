/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   pipex_utils.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/09/03 11:10:02 by aljulien  #+##+# */
/*   Updated: 2024/09/03 11:12:42 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

int handle_cat_process(int pipe_fd[2], t_line *line)
{
	pid_t pid;
	char buf[1024];

	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		if (read(STDIN_FILENO, buf, sizeof(buf)) > 0)
			write(STDOUT_FILENO, buf, ft_strlen(buf));
		cleanup(line);
		exit(0);
	}
	return (pid);
}

int handle_remaining_processes(int cat_count)
{
	int status;
	char buf[2];

	while (wait(&status) > 0)
		;
	if (cat_count != 0)
	{
		while (cat_count > 0)
		{
			read(STDIN_FILENO, buf, 1);
			cat_count--;
		}
	}
	return (1);
}

int _cat_count(t_pipe *current, t_line *line)
{
	int cat_count;

	cat_count = 0;
	while (current && ft_strcmp(current->arg[0], "cat") == 0
		&& current->arg[1] == NULL && !current->redir)
	{	
		cat_count++;
		current = current->next;
	}
	current = line->pipe;
	return (cat_count);
}

int handle_redirection(t_pipe *pipe)
{
	int saved_output;
	int redir_result;

	redir_result = redirection_in_pipe(pipe, &saved_output);
	if (redir_result == 0)
		return (0);
	if (redir_result == 2)
		return (2);
	return (1);
}