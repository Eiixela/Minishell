/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:25:40 by saperrie          #+#    #+#             */
/*   Updated: 2024/09/08 19:15:10 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env(t_env **env, char **envp)
{
	*env = NULL;
	create_env(envp, env);
	return (1);
}

void	initialize(t_line *line, int *status)
{
	line->skipped_char = 0;
	*status = 0;
}

int	setup_environment(t_env **env, char **envp, t_line *line)
{
	if (!init_env(env, envp))
		return (0);
	line->env = *env;
	return (1);
}

void	handle_exit(t_line *line, t_env *env)
{
	printf("exit\n");
	cleanup(line);
	free_env(env);
	exit(0);
}

void	update_status(int *status, t_line *line)
{
	if (g_ret == SIGINT)
		*status = 128 + g_ret;
	else
		*status = line->exit_status;
}
