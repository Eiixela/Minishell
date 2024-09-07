/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:06:49 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/07 17:07:34 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**arenvlst(t_env *env)
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
		arenv[i] = _strdup(tmp->env);
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

void	create_env(char **envp, t_env **env)
{
	size_t	i;
	t_env	*new;

	i = -1;
	new = NULL;
	new = env_newnode("");
	if (!new)
	{
		print_error(errno, "minishell: parsing");
		exit(EXIT_FAILURE);
	}
	env_addback(env, new);
	if (!envp || !*envp)
		return ;
	while (envp[++i])
	{
		new = env_newnode(envp[i]);
		if (!new)
		{
			env_freelst(env);
			print_error(errno, "minishell: parsing");
			exit(EXIT_FAILURE);
		}
		env_addback(env, new);
	}
}
