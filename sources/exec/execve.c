/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/06/07 15:36:40 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_cmd(char **env, char **cmd)
{
	char	*path;
	int i = 0;
	
	if (!cmd)
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return ;
	}
	path = get_path(*cmd, env, -1);
	i = execve(path, cmd, env);
}
