/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:35 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/31 17:44:28 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_cmd(char **env, char *cmd)
{
	char	*path;
	
	if (!cmd)
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		return ;
	}
	fprintf(stderr, "wesssh\n");
	path = get_path(cmd, env, -1);
	execve(path, &cmd, env);
}