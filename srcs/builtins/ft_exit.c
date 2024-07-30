/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:12:06 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 11:29:39 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lethal_exit(t_pipe *pipe, int arg_of_exit, char *error_message)
{
	(void)pipe;
	clear_history();
	/* if (y && (*lst) && (*lst)->env)
		env_freelst(&(*lst)->env);
	if (lst && (*lst))
		input_freelst(lst); */
	if (error_message)
		;
		//print_error(0, error_message);
	printf("exit");
	if (arg_of_exit > 0)
		exit(pipe->ret_val);
	else
		exit(EXIT_SUCCESS);
}

static bool	overflow(char *str)
{
	char	too_big[20];
	size_t	i;

	i = 0;
	ft_strlcpy(too_big, "9223372036854775807", 20);
	if (!str)
		return (true);
	if (ft_strlen(str) > ft_strlen(too_big))
		return (false);
	if (ft_strlen(str) == ft_strlen(too_big))
	{
		if (str[i] > too_big[i])
			return (false);
		i++;
	}
	return (true);
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

static int	print_error_message(char *s1, char *s2, char *s3)
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

static void	is_arg_valid(t_pipe *pipe, unsigned long long int arg_of_exit)
{
	if (not_num(pipe->arg[1]) == 0 || overflow(pipe->arg[1]) == 0)
	{
		print_error_message("minishell: exit: ", pipe->arg[1], \
		": numeric argument required");
		arg_of_exit = 2;
		lethal_exit(pipe, arg_of_exit, NULL);
	}
}


int ft_exit (t_pipe *pipe)
{
	unsigned long long int	arg_of_exit = 0;

	//if(line)
	//		free(line);
	if (pipe && pipe->arg[1])
	{
		is_arg_valid(pipe, 0);
		if (ft_dstrlen(pipe->arg) > 2)
			return(perror("minishell: exit "), 1);
		else
		{
			arg_of_exit = ft_atoll(pipe->arg[1]);
			pipe->ret_val = (unsigned char)arg_of_exit;
			return(lethal_exit(pipe, pipe->ret_val, NULL), pipe->ret_val);
		}
	}
	arg_of_exit = pipe->ret_val;
	return (lethal_exit(pipe, arg_of_exit, NULL), arg_of_exit);
}
