/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:21:45 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/12 17:42:20 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_env(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

/* int	env(t_input *in)
{
	t_env	*env;

	env = in->env;
	if (in->data[1])
		return (verror("minishell: ", "env: ", strerror(E2BIG)));
	while (env)
	{
		if (g_sig == SIGPIPE || ft_putendl_fd(env->env, STDOUT_FILENO) == -1)
			return (verror("minishell: ", "env: ", strerror(errno)));
		env = env->next;
	}
	return (0);
} */

/* typedef struct s_env
{
	char			*env;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env; */