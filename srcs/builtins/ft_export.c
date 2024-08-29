/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:08:05 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/29 15:28:50 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*exprt_inenv_export(t_env **env, char *data, bool has_equals)
{
	t_env	*new;

	new = env_newnode(data);
	if (!new)
		return (NULL);
	new->is_exported = !has_equals;
	if (!has_equals)
	{
		free(new->env);
		new->env = ft_strdup(data);
		if (!new->env)
		{
			free(new);
			return (NULL);
		}
	}
	env_addback(env, new);
	return (new);
}

static int	find_var_export(t_pipe **pipe, t_env *current, int rv,
		int arg_index)
{
	char	*s;
	char	*temp;

	while (current)
	{
		s = cut_string(current->env, '=');
		temp = cut_string((*pipe)->arg[arg_index], '=');
		if (!s || !temp)
			return (free_for_export(1, &s, &temp), -1);
		if (ft_strcmp(temp, s) == 0)
		{
			free(current->env);
			current->env = ft_strdup((*pipe)->arg[arg_index]);
			if (!current->env)
				return (free_for_export(2, &s, &temp), -1);
			current->is_exported = (rv != 1);
			return (free_for_export(2, &s, &temp), 1);
		}
		free_for_export(0, &s, &temp);
		current = current->next;
	}
	return (0);
}

static int	handles_find_in_export(int rv, t_pipe **pipe, t_env *env, int i)
{
	t_env	*new_node;

	new_node = exprt_inenv_export(&env, (*pipe)->arg[i], rv == 1);
	if (!new_node)
		return (1);
	new_node->is_exported = (rv == 0);
	return (0);
}

static int	exec_export(t_pipe **pipe, t_env *head, t_env *env, int status)
{
	int		rv;
	int		find_result;
	int		i;

	i = 0;
	while ((*pipe)->arg[++i])
	{
		rv = check_arg((*pipe)->arg[i]);
		if (rv >= 0)
		{
			find_result = find_var_export(pipe, head, rv, i);
			if (find_result == -1)
				return (1);
			if (find_result == 0)
				handles_find_in_export(rv, pipe, env, i);
			if (!head)
				head = env;
		}
		else if (rv == -1)
			status = 1;
		else if (rv == -2)
			status = 2;
	}
	return (status);
}

int	export(t_pipe **pipe, t_env *env)
{
	t_env	*head;
	int		status;

	status = 0;
	head = env;
	if (env && !(*pipe)->arg[1])
		if (sort_env(env) == 1)
			return (1);
	return (exec_export(pipe, head, env, status));
}
