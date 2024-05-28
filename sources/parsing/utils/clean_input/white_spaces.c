/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   white_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 18:15:23 by saperrie          #+#    #+#             */
/*   Updated: 2024/05/28 10:12:41 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

bool	is_white_space(char c)
{
	// if (c == '\0')
	// 	return (false);
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

void	skip_white_spaces(const char **input)
{
	if (!*input || !**input || !input)
		return ;
	while (is_white_space(**input))
		(*input)++;
}
