/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:38:12 by aljulien          #+#    #+#             */
/*   Updated: 2024/05/16 16:38:13 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd(void)
{
	char	*pwd;

	pwd = NULL;
	if (getcwd(pwd, sizeof(ft_strlen(pwd))))
	{
		ft_putstr_fd("cannot get pwd, sorry :(\n", STDOUT_FILENO);
		return (1);
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}