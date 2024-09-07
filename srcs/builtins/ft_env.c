/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_env.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/06/11 14:21:45 by aljulien  #+##+# */
/*   Updated: 2024/08/29 11:16:59 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

size_t	env_len(t_env *env)
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
	node->is_exported = 0;
	if (!node->env)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int	ft_env(t_env *env, t_pipe *pipe)
{
	t_env	*env_now;

	env_now = env;
	if (pipe->arg[1])
		return (print_error_message("minishell: ", "env: ", strerror(E2BIG)));
	while (env_now)
	{
		if (!env_now->is_exported && env_now->env && *(env_now->env))
		{
			if (g_ret == SIGPIPE \
				|| ft_putendl_fd(env_now->env, STDOUT_FILENO) == -1)
				return (print_error_message("minishell: ", "env: ", \
					strerror(errno)));
		}
		env_now = env_now->next;
	}
	return (0);
}
