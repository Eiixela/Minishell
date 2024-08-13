/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:18:07 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/12 11:10:52 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_nb_arg(char *arg)
{
	size_t	i;

	i = 0;
	if (arg[0] != '-')
		return (1);
	i++;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

//echo builting
//function return 0 if all went well, 1 if problems occured
int	ft_echo(char **arg)
{
	bool	indice_flag;
	int	i;

	i = 0;
	indice_flag = false;
	if (arg && arg[1])
	{
		while (arg[++i] && !count_nb_arg(arg[i]))
			indice_flag = true;
		while (arg && arg[i])
		{
			if (ft_putstr_fd(arg[i], STDOUT_FILENO) == -1)
				return (1);
			i++;
			if (arg[i])
				if (write(1, " ", 1) == -1)
					return (1);
		}
		if (indice_flag == false)
			if (write(1, "\n'", 1) == -1)
				return (1);
	}
	return (0);
}
