/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_lstadd_back_bonus.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2023/11/23 09:17:27 by aljulien  #+##+# */
/*   Updated: 2023/11/23 14:44:38 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (lst)
	{
		if (*lst)
		{
			last = ft_lstlast(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}
