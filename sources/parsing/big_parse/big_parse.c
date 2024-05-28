/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 19:22:22 by saperrie          #+#    #+#             */
/*   Updated: 2024/05/28 10:49:08 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

// TODO install todo tree / fix it

bool	big_parse(t_line *line, char **input)
{
	char	*str;

	if (!*input || !input)
		return (false);
	skip_white_spaces((const char **)input);
	if (!**input)
		return (false);
	str = *input;
	if (clean_input((const char **)&str))
		write(1, "CLEAN_INPUT\n", 13);
	else
		return (write(1, "BAD_INPUT\n", 11), false);
	if (lex((const char *)str, line))
		printf("GOOD_LEX\n");
	else
		return (printf("BAD_LEX\n"), false);
		// if (expand(line))
	// 	printf("CASH_MONEY\nargc = %i\n", line->argc);
	// else
	// 	return (printf("BROKE_ASS\n"), false);
		// if (parse(input))
		// 	printf("GOOD_PARSE\n")
		// check_path(intput);
	return (true);
}