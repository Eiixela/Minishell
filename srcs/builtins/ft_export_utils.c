/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:31:33 by aljulien          #+#    #+#             */
/*   Updated: 2024/08/29 14:02:58 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

ssize_t	print_senv(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		if (printf("declare -x %s\n", arr[i]) == -1)
			return (-1);
		i++;
	}
	return (1);
}

char	*cut_string(const char *input_string, char cut_char)
{
	char	*result;
	char	*cut_position;
	size_t	length;

	cut_position = strchr(input_string, cut_char);
	if (cut_position != NULL)
	{
		length = cut_position - input_string;
		result = (char *)malloc(length + 1);
		if (result == NULL)
			return (fprintf(stderr, "Memory allocation failed\n"), NULL);
		strncpy(result, input_string, length);
		result[length] = '\0';
	}
	else
	{
		result = strdup(input_string);
		if (result == NULL)
			return (fprintf(stderr, "Memory allocation failed\n"), NULL);
	}
	return (result);
}

static bool	is_sorted(char **tab)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_tablen(tab);
	if (tab)
	{
		while (i <= j - 1)
		{
			if (ft_strncmp(tab[i], tab[i + 1], 1) > 0)
				return (false);
			i++;
		}
	}
	return (true);
}

static char	**sort_tab(char **arenv, size_t len)
{
	size_t	i;
	size_t	j;
	char	*tenv;

	i = 0;
	while (i < 100 && arenv[i])
	{
		j = i + 1;
		if (j <= len && ft_strncmp(arenv[i], arenv[j], ft_strlen(arenv[i])) > 0)
		{
			tenv = arenv[i];
			arenv[i] = arenv[j];
			arenv[j] = tenv;
		}
		i++;
	}
	return (arenv);
}

int	sort_env(t_env	*env)
{
	char	**arenv;
	size_t	len;
	ssize_t	rv;

	arenv = arenvlst(env);
	if (!arenv)
		return (verror("minishell: ", "export: ", strerror(errno)));
	len = ft_arrlen(arenv);
	while (!is_sorted(arenv))
		arenv = sort_tab(arenv, len);
	rv = print_senv(arenv);
	free_dtab(arenv);
	if (rv == -1)
		return (1);
	return (0);
}
