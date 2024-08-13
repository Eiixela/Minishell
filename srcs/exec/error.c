/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:31:05 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/13 12:33:34 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*errjoin(int error_code, char *error_message)
{
	char	*error;
	char	*tmp;

	tmp = ft_strjoin(error_message, ": ");
	if (!tmp)
		return (print_error(0, "Congrats ! The error message crashed."));
	error = ft_strjoin(tmp, strerror(error_code));
	if (!error)
		return (print_error(0, "Congrats ! The error message crashed."));
	free(tmp);
	ft_putstr_fd("\033[1;31m", STDERR_FILENO);
	ft_putendl_fd(error, STDERR_FILENO);
	free(error);
	ft_putstr_fd("\033[0m", STDERR_FILENO);
	return ((int *)true);
}

void	*print_error(int error_code, char *error_message)
{
	if (error_code && !error_message)
	{
		ft_putstr_fd("\033[1;31m", STDERR_FILENO);
		ft_putendl_fd(strerror(error_code), STDERR_FILENO);
		ft_putstr_fd("\033[0m", STDERR_FILENO);
	}
	else if (!error_code && error_message)
	{
		ft_putstr_fd("\033[1;31m", STDERR_FILENO);
		ft_putendl_fd(error_message, STDERR_FILENO);
		ft_putstr_fd("\033[0m", STDERR_FILENO);
	}
	else if (error_code && error_message)
		errjoin(error_code, error_message);
	return (NULL);
}