/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saperrie <saperrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 01:03:34 by marvin            #+#    #+#             */
/*   Updated: 2024/08/14 12:02:16 by saperrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	str_contains_wspace(char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '\t')
			return (true);
	}
	return (false);
}

int		get_namelen(char *input)
{
	int	length;

	length = 0;
	while (*input)
	{
		if (ft_isalnum(*input) || *input == '_')
			length += 1;
		else
			break ;
		input++;
	}
	return (length);
}

static char	*get_env_value(t_line *line, char *name)
{
	char	**env;
	char	*value;
	int		name_len;

	env = line->env;
	name_len = ft_strlen(name);
	while (*env)
	{
		if (!ft_strncmp(name, *env, name_len) && *(*env + name_len) == '=')
		{
			value = ft_strdup(*env + name_len + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		env++;
	}
	return (ft_calloc(1, 1));
}

t_line	*make_new_nodes(char **new_nodes, t_line *line)
{
	while (*new_nodes)
	{
		line = make_argv_node(*new_nodes, ft_strlen(*new_nodes), line);
		if (!line)
			return (NULL);
		(*new_nodes)++;
	}
	return (line);
}

// FIXME Handle single $ (or $+ $= )
//							 + = / . , : ~ ` ! # $ % ^ ( ) { } [ ] PRINT DOLLAR

char	*which_expand(char *input, t_line *line)
{
	char	*name;
	char	*value;
	char	**new_nodes;
	int		namelen;

	namelen = get_namelen(input);
	if (namelen == -1)
		return (ft_calloc(1, 1));
	++input;
	// if (namelen == 0)
	// {
	// 	line = make_argv_node("$", 1, line);
	// 	if (!line)
	// 		return (NULL);
	// 	return (input);
	// }
	name = ft_substr(input, 0, namelen);
	if (!name)
		return (NULL);
	value = get_env_value(line, name);
	if (!value)
		return (NULL);
	if (name)
		free(name);
	if (!str_contains_wspace(value))
	{
		line = make_argv_node(value, namelen, line);
		if (!line)
			return (NULL);
	}
	else
	{
		new_nodes = ft_split(value, ' ');
		if (!new_nodes)
			return (NULL);
		line = make_new_nodes(new_nodes, line);
		if (!line)
			return (NULL);
	}
	return (input + namelen);
}

char	*super_expand(char *input, t_line *line)
{
	if (input[0] == '$' && (!ft_isalnum(input[1]) || input[1] != '_')
}
