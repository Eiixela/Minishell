/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/11 18:15:44 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_cmd(char **env, char **cmd) 
{
	char	*path;
	
	if (!cmd || !cmd[0]) 
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		return ;
	}
	path = get_path(cmd[0], env, -1);
	if (!path) 
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd[0]);
		return ;
	}
	if (execve(path, cmd, env) == -1)
		perror("execve");
}
