/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_free_dtab.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/08/13 12:22:05 by aljulien  #+##+# */
/*   Updated: 2024/08/13 12:22:08 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	free_dtab(char **dtab)
{
	size_t	i;

	i = 0;
	if (!dtab)
		return ;
	while (dtab[i])
	{
		free(dtab[i]);
		i++;
	}
	free(dtab);
	dtab = NULL;
	return ;
}