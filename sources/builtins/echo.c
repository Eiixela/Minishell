/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:18:07 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/17 16:37:27 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write (fd, &s[i], 1);
		i++;
	}
}

static int	count_nb_arg(char **arg)
{
	int	nb_arg;
	
	nb_arg = 0;
	while(arg[nb_arg])
		nb_arg++;
	return (nb_arg);
}

//echo builting
//function return 0 if all went well, 1 if problems occured
int	ft_echo(char **arg)
{
	int	indice_flag;
	int	i;

	i = 1;
	indice_flag = 0;
	if (count_nb_arg(arg) > 1)
	{
		 while (ft_strncmp(arg[i], "-n", 2) == 0)
		{
			indice_flag = 1;
			i++;
		}
		while (arg[i])
		{
			ft_putstr_fd(arg[i], 1);
			if (arg[i + 1] && arg[i][0] != '\0')
				write (1, " ", 1);
			i++;
		}
	}
	if (indice_flag == 0)
		write(1, "\n", 1);
	return (0);
}