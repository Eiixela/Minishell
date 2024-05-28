/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/28 15:18:31 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_cmd(char **env, t_line line)
{
	char	*path;
	
	if (!line.argv->av && !line.argv->av[0])
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return ;
	}
	path = get_path(line.argv->av, env, -1);
	execve(path, &line.argv->av, env);
}