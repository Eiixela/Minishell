/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:21:45 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/13 12:25:32 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	env_len(t_env *env)
{
	t_env	*tmp;
	size_t	n_nodes;

	tmp = env;
	n_nodes = 0;
	while (tmp)
	{
		n_nodes++;
		tmp = tmp->next;
	}
	return (n_nodes);
}

char	**arenvlst(t_env	*env)
{
	char	**arenv;
	size_t	i;
	t_env	*tmp;

	tmp = env;
	if (!tmp)
		return (NULL);
	arenv = NULL;
	i = 0;
	arenv = malloc(sizeof(char *) * (env_len(env) + 1));
	if (!arenv)
		return (print_error(errno, "minishell"));
	arenv[env_len(env)] = 0;
	while (tmp)
	{
		arenv[i] = ft_strdup(tmp->env);
		if (!arenv[i])
		{
			free_dtab(arenv);
			return (print_error(errno, "minishell"));
		}
		i++;
		tmp = tmp->next;
	}
	return (arenv);
}

void	env_freelst(t_env **env)
{
	t_env	*tmp;

	tmp = NULL;
	if (env && (*env))
	{
		while (*env)
		{
			free((*env)->env);
			tmp = (*env)->next;
			free(*env);
			(*env) = tmp;
		}
	}
	*env = NULL;
	env = NULL;
}

void	env_addback(t_env **env, t_env *node)
{
	t_env	*head;

	if (!node || !env)
		return ;
	if (!*env)
		*env = node;
	else
	{
		head = (*env);
		while ((*env)->next)
			(*env) = (*env)->next;
		(*env)->next = node;
		node->prev = (*env);
		(*env) = head;
	}
}


t_env	*env_newnode(char *data)
{
	t_env	*node;

	node = malloc(1 * sizeof(t_env));
	if (!node)
		return (NULL);
	node->env = ft_strdup(data);
	if (!node->env)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	create_env(char **envp, t_env **env)
{
	size_t		i;
	t_env		*new;

	i = 0;
	new = NULL;
	if (!envp || !*envp)
		return ;
	while (envp[i])
	{
		new = env_newnode(envp[i]);
		if (!new)
		{
			env_freelst(env);
			print_error(errno, "minishell: parsing");
			exit(EXIT_FAILURE);
		}
		env_addback(env, new);
		i++;
	}
}

int	ft_env(t_env *env, t_pipe *pipe)
{
	t_env	*env_now;

	env_now = env;
	if (pipe->arg[1])
		return (print_error_message("minishell: ", "env: ", strerror(E2BIG)));
	while (env_now)
	{
		if (g_ret == SIGPIPE || ft_putendl_fd(env_now->env, STDOUT_FILENO) == -1)
			return (print_error_message("minishell: ", "env: ", strerror(errno)));
		env_now = env_now->next;
	}
	return (0);
}
