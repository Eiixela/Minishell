/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:12:13 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/22 11:29:33 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*comp_keys(char *to_find, char *key)
{
	size_t	i;

	if (!to_find || !key)
		return (NULL);
	i = 0;
	while (to_find[i] && key[i])
	{
		if (to_find[i] != key[i])
			return (NULL);
		i++;
	}
	if (to_find[i] != '=')
		return (NULL);
	return (to_find);
}

static	size_t	env_len(t_env *env)
{
	size_t	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

static t_env	*env_rmone(t_env **sup, t_env **head)
{
	t_env	*tmp;

	if (!(*sup))
		return (print_error(errno, "minishell: exec"));
	tmp = (*sup);
	if (!tmp->prev)
	{
		(*sup) = (*sup)->next;
		if (env_len(tmp) > 1)
			(*sup)->prev = NULL;
		head = sup;
	}
	else if ((*sup) && !tmp->next)
		(*sup)->prev->next = NULL;
	else
	{
		(*sup)->prev->next = (*sup)->next;
		(*sup)->next->prev = (*sup)->prev;
	}
	sup = head;
	free(tmp->env);
	free(tmp);
	return (*sup);
}

int	ft_unset(t_line **line, t_env *env)
{
	t_env	*head;
	size_t	i;

	if (!line || !(*line) || !env || ((*line)->pipe->arg && !(*line)->pipe->arg[1]))
		return (1);
	head = env;
	i = 1;
	while ((*line)->pipe->arg[i])
	{
		while (env)
		{
			if (comp_keys(env->env, (*line)->pipe->arg[i]))
			{
				head = env_rmone(&env, &head);
				break ;
			}
			env = env->next;
		}
		env = head;
		i++;
	}
	env = head;
	return (0);
}