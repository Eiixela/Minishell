/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:31:05 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/23 13:19:41 by aljulien         ###   ########.fr       */
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

int	verror(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*error_message;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (print_error(0, "Congrat ! The error message crashed"), 1);
	error_message = ft_strjoin(tmp, s3);
	if (!error_message)
		return (free(tmp), \
		print_error(0, "Congrat ! The error message crashed"), 1);
	free(tmp);
	if (ft_putstr_fd("\033[1;31m", STDERR_FILENO) == -1)
		return (-1);
	if (ft_putstr_fd(error_message, STDERR_FILENO) == -1)
		return (-1);
	free(error_message);
	if (ft_putendl_fd("\033[0m", STDERR_FILENO) == -1)
		return (-1);
	return (1);
}
