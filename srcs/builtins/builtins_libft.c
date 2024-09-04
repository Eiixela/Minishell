/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_libft.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:20 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/04 13:33:15 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr || !*arr)
		return (i);
	while (arr[i])
		i++;
	return (i);
}
