/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:25:40 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/07 21:26:06 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env(t_env **env, char **envp)
{
	*env = NULL;
	create_env(envp, env);
	siglisten();
	return (1);
}

void	initialize_variables(t_line *line, int *status)
{
	line->skipped_char = 0;
	*status = 0;
}

int	initialize_environment(t_env **env, char **envp)
{
	if (!init_env(env, envp))
		return (0);
	return (1);
}
