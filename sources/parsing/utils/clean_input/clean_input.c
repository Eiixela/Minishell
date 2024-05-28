/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:10:46 by saperrie          #+#    #+#             */
/*   Updated: 2024/05/28 10:12:26 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

bool	clean_input(const char **str)
{
	skip_white_spaces((const char **)str);
	if (!*str)
		return (false);
	if (!quotes(*str))
		return (false);
	if (!good_redirections(*str))
		return (false);
	return (true);
}
