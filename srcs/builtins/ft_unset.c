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

static	size_t	env_len(t_env *env)
{
	size_t	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
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
		if (env_len(tmp) > 1)
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

int is_valid_identifier(const char *str) {
if (!str || !*str) return 0;

if (!isalpha(*str) && *str != '_') return 0;

for (str++; *str; str++) {
if (!isalnum(*str) && *str != '_') return 0;
}

return 1;
}

int ft_unset(t_line **line, t_env *env)
{
t_env *head;
size_t i;
int status = 0;

if (!line || !(*line) || !env || ((*line)->pipe->arg && !(*line)->pipe->arg[1]))
return 1;

head = env;
i = 1;
while ((*line)->pipe->arg[i])
{
if ((*line)->pipe->arg[i][0] == '-')
{
fprintf(stderr, "bash: unset: --: invalid option\n");
return 2;
}

if (!is_valid_identifier((*line)->pipe->arg[i]))
{
fprintf(stderr, "bash: unset: `%s': not a valid identifier\n", (*line)->pipe->arg[i]);
status = 1;
}
else
{
env = head;
while (env)
{
if (comp_keys(env->env, (*line)->pipe->arg[i]))
{
head = env_rmone(&env, &head);
break;
}
env = env->next;
}
}
i++;
}
env = head;
return status;
}

