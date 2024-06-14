/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:21:05 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/14 10:21:17 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parse_builtin(char **env, t_line line)
{

	(void)env;
	if (!ft_strcmp(line.pipe->arg[0], "echo"))
		ft_echo(line.pipe->arg);
	if (!ft_strcmp(line.pipe->arg[0], "cd"))
		ft_cd(line.pipe->arg, env);
	if (!ft_strcmp(line.pipe->arg[0], "pwd"))
		ft_pwd(line.pipe->arg);
	if (!ft_strcmp(line.pipe->arg[0], "export"))
		printf("export\n");	//ft_export()
	if (!ft_strcmp(line.pipe->arg[0], "unset"))
		printf("unset\n");	//ft_unset()
	if (!ft_strcmp(line.pipe->arg[0], "env"))
		ft_env(env);
	if (!ft_strcmp(line.pipe->arg[0], "exit"))
		printf("exit\n");	//ft_exit
	else 
		return (0);
	return (1);
}