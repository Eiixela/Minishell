/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_calloc.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/11 21:08:58 by alexie#+##+# */
/*   Updated: 2023/11/27 16:50:18 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	total;

	if (nmemb == 0 || size == 0)
		return (malloc (0));
	total = nmemb * size;
	if (nmemb != 0 && ((total / nmemb) != size))
		return (0);
	p = malloc (total);
	if (!p)
		return (NULL);
	ft_bzero(p, total);
	return (p);
}
