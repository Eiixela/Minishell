/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:21:34 by saperrie          #+#    #+#             */
/*   Updated: 2024/06/11 17:26:25 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	redirection_offset(char redir_operator)
{
	if (redir_operator == HEREDOC || redir_operator == APPEND)
		return (2);
	else if (redir_operator == IN_REDIR || redir_operator == OUT_REDIR)
		return (1);
	return (0);
}

char	skip_redirection_operator(char **str)
{
	if ((*str)[0] == '<' && (*str)[1] == '<')
		return (*(str) += 2, HEREDOC);
	else if ((*str)[0] == '>' && (*str)[1] == '>')
		return (*(str) += 2, APPEND);
	else if (**str == '>')
		return (*(str) += 1, OUT_REDIR);
	else if (**str == '<')
		return (*(str) += 1, IN_REDIR);
	return (false);
}

char	is_redirection_operator(char *str)
{
	if (str[0] == '<' && str[1] == '<')
		return (HEREDOC);
	else if (str[0] == '>' && str[1] == '>')
		return (APPEND);
	else if (*str == '>')
		return (OUT_REDIR);
	else if (*str == '<')
		return (IN_REDIR);
	return (0);
}
