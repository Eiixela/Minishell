/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   heredoc.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: aljulien <aljulien@student.42.fr>  +#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2024/09/03 11:15:01 by aljulien  #+##+# */
/*   Updated: 2024/09/03 11:15:44 by aljulien ###   ########.fr   */
/**/
/* ************************************************************************** */

#include "minishell.h"

static char	*gen_filename(int fn)
{
	char	*strfn;
	char	*filename;

	strfn = ft_itoa(fn);
	if (!strfn)
		return (print_error(errno, "minishell: heredoc"));
	filename = ft_strjoin("/tmp/tmp_", strfn);
	free(strfn);
	if (!filename)
		return (print_error(errno, "minishell: heredoc"));
	return (filename);
}

// Function to find the value of a variable in the custom environment list
static char *get_env_value_heredoc(t_env *env, const char *var_name)
{
	t_env *current;
	size_t var_name_len;

	var_name_len = ft_strlen(var_name);
	current = env;
	while (current)
	{
		if (ft_strncmp(current->env, var_name, var_name_len) == 0 && current->env[var_name_len] == '=')
			return (current->env + var_name_len + 1);
		current = current->next;
	}
	return (NULL);
}

// Function to expand variables in a string using the custom environment list
char *expand_variables(const char *input, t_env *env)
{
    char *result;
    char *var_start, *var_end;
    char *var_name, *var_value;
    size_t new_size, current_len;
    char *new_result;

    result = ft_strdup(input);
    while ((var_start = ft_strchr(result, '$')) != NULL)
    {
        var_end = var_start + 1;
        while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
            var_end++;
        var_name = ft_strndup(var_start + 1, var_end - var_start - 1);
        var_value = get_env_value_heredoc(env, var_name);
        if (var_value)
        {
            new_size = ft_strlen(result) - ft_strlen(var_name) + ft_strlen(var_value);
            new_result = malloc(new_size + 1);
            if (!new_result) 
                return (free(var_name), free(result), perror("malloc"), NULL);
            current_len = var_start - result;
            ft_strlcpy(new_result, result, current_len + 1);
            ft_strlcat(new_result, var_value, new_size + 1);
            ft_strlcat(new_result, var_end, new_size + 1);
            free(result);
            result = new_result;
        }
        free(var_name);
    }
    return (result);
}


static int handle_single_heredoc(char *delimiter, const char *temp_file, t_env *env)
{
	int fd_file_heredoc;
	char *line_heredoc;
	char *expanded_line;

	fd_file_heredoc = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_file_heredoc == -1)
		return (ft_putstr_fd("error: cannot open file\n", 2), 0);
		
	while (1)
	{
		line_heredoc = readline("> ");
		if (!line_heredoc || ft_strcmp(line_heredoc, delimiter) == 0)
		{
			free(line_heredoc);
			break;
		}
		expanded_line = expand_variables(line_heredoc, env);
		if (!expanded_line)
		{
			free(line_heredoc);
			return (ft_putstr_fd("error: cannot expand variable\n", 2), 0);
		}
		write(fd_file_heredoc, expanded_line, ft_strlen(expanded_line));
		write(fd_file_heredoc, "\n", 1);
		free(expanded_line);
		free(line_heredoc);
	}
	close(fd_file_heredoc);
	return (1);
}

char *ensure_positive_chars(char* str)
{
    size_t i = 0;
    while (str[i] != '\0') 
	{
        if (str[i] < 0)
            str[i] = (unsigned char)str[i];
        i++;
    }
    return (str);
}

// Modified function to handle multiple heredocs
int redir_heredoc(t_pipe *pipe, t_env *env)
{
	char *temp_file;
	int heredoc_count = 0;
	int fd_file_heredoc;
	t_redir *current_redir = pipe->redir;

	while (current_redir && current_redir->type == HEREDOC)
	{
		temp_file = gen_filename(heredoc_count);
		current_redir->fd = ensure_positive_chars(current_redir->fd);
		if (!handle_single_heredoc(current_redir->fd, temp_file, env))
			return (0);
		current_redir = current_redir->next;
		heredoc_count++;
	}
	fd_file_heredoc = open(temp_file, O_RDONLY);
	if (fd_file_heredoc == -1)
		return (perror("error on open for reading"), 0);
	if (dup2(fd_file_heredoc, STDIN_FILENO) == -1)
		return (perror("dup2 heredoc"), 0);
	close(fd_file_heredoc);
	for (int i = 0; i < heredoc_count; i++)
		unlink(temp_file);
	free(temp_file);
	return (1);
}
