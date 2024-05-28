/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:55:40 by saperrie          #+#    #+#             */
/*   Updated: 2024/05/28 10:12:07 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static bool	clean_surrounding_quotes(t_line *line)
{
	(void)line;
	return (true);
}

bool	parse(t_line *line)
{
	if (!clean_surrounding_quotes(line))
		return (printf("clean_quotes_failed"), false);
	// tag_slots(line);
	return (true);
}
