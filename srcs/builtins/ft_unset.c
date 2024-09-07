/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_unset.c :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/06/11 15:12:13 by aljulien  #+##+# */
/*   Updated: 2024/08/28 13:40:12 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

static char	*comp_keys(char *to_find, char *key)
{
	size_t	i;

	if (!to_find || !key)
		return (NULL);
	i = 0;
	while (to_find[i] && key[i])
	{
		if (to_find[i] != key[i])
			return (NULL);
		i++;
	}
	if (to_find[i] != '=')
		return (NULL);
	return (to_find);
}

static t_env	*env_rmone(t_env **sup, t_env **head)
{
	t_env	*tmp;

	if (!(*sup))
		return (print_error(errno, "minishell: exec"));
	tmp = (*sup);
	if (!tmp->prev)
	{
		(*sup) = (*sup)->next;
		if (env_len_unset(tmp) > 1)
			(*sup)->prev = NULL;
		head = sup;
	}
	else if ((*sup) && !tmp->next)
		(*sup)->prev->next = NULL;
	else
	{
		(*sup)->prev->next = (*sup)->next;
		(*sup)->next->prev = (*sup)->prev;
	}
	sup = head;
	free(tmp->env);
	free(tmp);
	return (*sup);
}

t_env	*ft_remove_env_var(t_env **head, char *arg)
{
	t_env	*env;

	env = *head;
	while (env)
	{
		if (comp_keys(env->env, arg))
		{
			*head = env_rmone(&env, head);
			break ;
		}
		env = env->next;
	}
	return (*head);
}

int	ft_unset(t_line **line, t_env *env)
{
	t_env	*head;
	size_t	i;

	if (!line || !(*line) || !env)
		return (0);
	if ((*line)->pipe->arg && !(*line)->pipe->arg[1])
		return (0);
	head = env;
	i = 1;
	while ((*line)->pipe->arg[i])
	{
		if ((*line)->pipe->arg[i][0] == '-')
			return (ft_putstr_fd("bash: unset: --: invalid option\n", 2), 2);
		if (!is_valid_identifier((*line)->pipe->arg[i]))
			return (print_error_message("bash: unset: `",
					(*line)->pipe->arg[i], "': not a valid identifier\n"), 1);
		env = ft_remove_env_var(&head, (*line)->pipe->arg[i]);
		i++;
	}
	return (0);
}
