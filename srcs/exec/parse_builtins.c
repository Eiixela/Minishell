/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:41:10 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 10:56:48 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_builtin (t_pipe *pipe)
{
	if (!ft_strcmp(pipe->arg[0], "echo"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "cd"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "pwd"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "export"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "unset"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "env"))
		return (1);
	else if (!ft_strcmp(pipe->arg[0], "exit"))
		return (1);
	else
		return (0);
}
