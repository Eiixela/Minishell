/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 09:15:02 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/04 09:38:21 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	env_len_unset(t_env *env)
{
	size_t	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

int	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (0);
	if (!isalpha(*str) && *str != '_')
		return (0);
	str++;
	while (*str)
	{
		if (!isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}
