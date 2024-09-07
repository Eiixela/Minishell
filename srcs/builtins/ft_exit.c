/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   ft_exit.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/06/11 15:12:06 by aljulien  #+##+# */
/*   Updated: 2024/09/03 10:21:21 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

void	lethal_exit(int arg_of_exit, char *error_message, \
	t_line *line, int indice)
{
	clear_history();
	free_env(line->env);
	cleanup(line);
	if (error_message)
		print_error(0, error_message);
	if (indice == 0)
		printf("exit\n");
	if (arg_of_exit > 0)
		exit(arg_of_exit);
	else
		exit(EXIT_SUCCESS);
}

static bool	overflow(const char *str)
{
	char	*max;
	char	*min;
	size_t	len;

	len = ft_strlen(str);
	min = "-9223372036854775808";
	max = "9223372036854775807";
	if (str[0] == '-')
		return (len <= ft_strlen(min) && ft_strcmp(str, min) <= 0);
	else
		return (len <= ft_strlen(max) && ft_strcmp(str, max) <= 0);
}

static bool	not_num(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(str[i]) || str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	print_error_message(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*error_message;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (printf("Congrat ! The error message crashed"));
	error_message = ft_strjoin(tmp, s3);
	if (!error_message)
		return (free(tmp), \
		printf("Congrat ! The error message crashed"));
	free(tmp);
	if (ft_putstr_fd(error_message, STDERR_FILENO) == -1)
		return (-1);
	free(error_message);
	return (1);
}

int	ft_exit(t_pipe *pipe, t_line *line)
{
	long long	arg_of_exit;

	arg_of_exit = 0;
	if (pipe && pipe->arg[1])
	{
		if (!not_num(pipe->arg[1]) || !overflow(pipe->arg[1]))
		{
			printf("exit\n");
			print_error_message("minishell: exit: ", pipe->arg[1],
				": numeric argument required\n");
			lethal_exit(2, NULL, line, 1);
		}
		if (ft_dstrlen(pipe->arg) > 2)
			return (printf("exit\nbash: exit: too many arguments\n"), 1);
		arg_of_exit = ft_atoll(pipe->arg[1]);
		pipe->ret_val = (unsigned char)arg_of_exit;
		lethal_exit(pipe->ret_val, NULL, line, 0);
		return (pipe->ret_val);
	}
	arg_of_exit = pipe->ret_val;
	lethal_exit(arg_of_exit, NULL, line, 0);
	return (arg_of_exit);
}
