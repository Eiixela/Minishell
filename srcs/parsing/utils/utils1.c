/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   utils.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: marvin <marvin@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/05/21 22:10:09 by saperrie  #+##+# */
/*   Updated: 2024/08/29 02:55:02 by marvin   ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

void	get_value_back(char *str)
{
	while (*str)
	{
		if (*str < 0)
			*str *= -1;
		str += 1;
	}
}
