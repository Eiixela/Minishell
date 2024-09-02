/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 21:05:51 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/02 11:47:14 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir *redir)
{
	t_redir	*current;
	t_redir	*next;

	current = redir;
	while (current)
	{
		next = current->next;
		free(current->fd);
		free(current);
		current = next;
	}
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->env);
		free(current);
		current = next;
	}
}

void	free_pipe(t_pipe *pipe)
{
	t_pipe	*current;
	t_pipe	*next;
	int		i;

	current = pipe;
	while (current)
	{
		next = current->next;
		if (current->arg)
		{
			i = 0;
			while (current->arg[i])
				free(current->arg[i++]);
			free(current->arg);
		}
		free_redir(current->redir);
		free(current);
		current = next;
	}
}

void	free_argv(t_argv *argv)
{
	t_argv	*current;
	t_argv	*next;

	current = argv;
	while (current != NULL)
	{
		next = current->next;
		if (current->node)
			free(current->node);
		free(current);
		current = next;
	}
}

void	cleanup(t_line *line)
{
	if (line->pipe)
	{
		while (line->pipe)
		{
			if (line->pipe->redir && line->pipe->redir_head)
				line->pipe->redir = line->pipe->redir_head;
			line->pipe = line->pipe->next;
		}
		if (line->pipe_head)
			line->pipe = line->pipe_head;
	}
	if (line->argv)
		line->argv = line->argv_head;
	//free_argv(line->argv);
	free_pipe(line->pipe);
	line->argv = NULL;
	line->pipe = NULL;
}
