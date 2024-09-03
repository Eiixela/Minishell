/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_lstadd_front_bonus.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42lyon.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/23 09:01:48 by aljulien  #+##+# */
/*   Updated: 2023/11/23 09:12:04 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst)
	{
		if (*lst)
			new->next = *lst;
		*lst = new;
	}
}
