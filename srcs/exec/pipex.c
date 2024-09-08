/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 pipex.c											:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: aljulien <aljulien@student.42.fr>			+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/05/28 11:48:55 by aljulien		   #+#	  #+#			  */
/*	 Updated: 2024/09/03 11:12:49 by aljulien		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	setup_io(t_io_fds *fds)
{
	if (fds->input_fd != STDIN_FILENO)
	{
		if (dup2(fds->input_fd, STDIN_FILENO) == -1)
			return (perror("dup2 input_fd"), 0);
		close(fds->input_fd);
	}
	if (fds->output_fd != STDOUT_FILENO)
	{
		if (dup2(fds->output_fd, STDOUT_FILENO) == -1)
			return (perror("dup2 output_fd"), 0);
		close(fds->output_fd);
	}
	return (1);
}

static int	handle_input_redirection(char *filename, t_line *line, t_env *env)
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

static int	handle_output_redirection(char *filename, t_redir *redir,
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

static int	handle_heredoc(const char *filename, t_line *line, t_env *env)
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

static void	apply_redirection(int *fd, int target_fd, t_line *line, t_env *env)
{
	if (*fd != target_fd)
	{
		if (dup2(*fd, target_fd) == -1)
		{
			perror("dup2");
			close(*fd);
			cleanup(line);
			free_env(env);
			exit(1);
		}
		close(*fd);
		*fd = target_fd;
	}
}

static void	handle_redir_type(t_redir *redir, t_io_fds *fds,
				t_line *line, t_env *env)
{
	if (redir->type == IN_REDIR)
	{
		if (fds->input_fd != STDIN_FILENO)
			close(fds->input_fd);
		fds->input_fd = handle_input_redirection(redir->fd, line, env);
	}
	else if (redir->type == OUT_REDIR || redir->type == APPEND)
	{
		if (fds->output_fd != STDOUT_FILENO)
			close(fds->output_fd);
		fds->output_fd = handle_output_redirection(redir->fd, redir, line, env);
	}
	else if (redir->type == HEREDOC)
	{
		if (fds->input_fd != STDIN_FILENO)
			close(fds->input_fd);
		fds->input_fd = handle_heredoc(redir->fd, line, env);
	}
}

void	while_redir(t_line *line, t_pipe *pipe, t_env *env)
{
	t_redir		*redir;
	t_io_fds	fds;

	fds.input_fd = STDIN_FILENO;
	fds.output_fd = STDOUT_FILENO;
	redir = pipe->redir;
	while (redir)
	{
		handle_redir_type(redir, &fds, line, env);
		redir = redir->next;
	}
	apply_redirection(&fds.input_fd, STDIN_FILENO, line, env);
	apply_redirection(&fds.output_fd, STDOUT_FILENO, line, env);
}

static void	setup_child_io(t_io_fds *fds, int pipe_fd)
{
	if (pipe_fd > -1)
		close(pipe_fd);
	if (fds->input_fd != STDIN_FILENO)
	{
		if (dup2(fds->input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(1);
		}
		close(fds->input_fd);
	}
	if (fds->output_fd != STDOUT_FILENO)
	{
		if (dup2(fds->output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(1);
		}
		close(fds->output_fd);
	}
}

int	create_process(t_process_info *info, t_io_fds *fds, t_pipe *pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_io(fds, info->pipe_fd);
		while_redir(info->line, pipe, info->env);
		if (pipe->arg && pipe->arg[0])
			execute_cmd(info->env, pipe, info->line, info->str);
		free_env(info->env);
		cleanup_exec(info->line);
	}
	return (pid);
}

int	process_pipe(t_process_info *info, t_pipe *current, int *input_fd,
		int cat_count)
{
	int			pipe_fd[2];
	pid_t		pid;
	t_io_fds	fds;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	fds.input_fd = *input_fd;
	fds.output_fd = pipe_fd[1];
	if (cat_count > 0)
	{
		pid = handle_cat_process(pipe_fd, info->line);
		cat_count--;
	}
	else
	{
		info->pipe_fd = pipe_fd[0];
		pid = create_process(info, &fds, current);
	}
	close(pipe_fd[1]);
	if (*input_fd != 0)
		close(*input_fd);
	*input_fd = pipe_fd[0];
	return (pid);
}

static int	count_cat_commands(t_pipe *current)
{
	int	count;

	count = 0;
	while (current && current->arg && ft_strcmp(current->arg[0], "cat") == 0
		&& current->arg[1] == NULL && !current->redir)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	process_commands(t_process_info *info, int *input_fd,
		int cat_count, pid_t *last_pid)
{
	t_pipe		*current;
	pid_t		pid;
	t_io_fds	fds;

	current = info->line->pipe;
	cat_count = count_cat_commands(current);
	current = info->line->pipe;
	while (current != NULL)
	{
		if (current->next != NULL)
			pid = process_pipe(info, current, input_fd, cat_count);
		else
		{
			fds.input_fd = *input_fd;
			fds.output_fd = STDOUT_FILENO;
			info->pipe_fd = -1;
			pid = create_process(info, &fds, current);
			if (*input_fd != 0)
				close(*input_fd);
		}
		*last_pid = pid;
		current = current->next;
	}
	return (1);
}

int	call_childs(t_env *env, t_line *line, char *str, pid_t last_pid)
{
	int				input_fd;
	int				cat_count;
	pid_t			wpid;
	t_process_info	info;

	input_fd = 0;
	cat_count = parse_and_execute_solo_builtins(env, line, -1);
	if (cat_count != 1)
		return (cat_count);
	cat_count = 0;
	info.env = env;
	info.line = line;
	info.str = str;
	if (!process_commands(&info, &input_fd, cat_count, &last_pid))
		return (0);
	sigend();
	while (1)
	{
		wpid = wait(&line->exit_status);
		if (wpid <= 0)
			break ;
		if (wpid == last_pid)
			handle_exit_status_child(line, line->exit_status, 0, &cat_count);
	}
	return (handle_remaining_processes(cat_count));
}

int	pipex(t_env *env, t_line *line, int *status, char *str)
{
	pid_t			last_pid;

	last_pid = 0;
	line->pipe->ret_val = *status;
	if (!process_heredocs(line, env))
		return (1);
	if (!call_childs(env, line, str, last_pid))
		return (0);
	return (1);
}
