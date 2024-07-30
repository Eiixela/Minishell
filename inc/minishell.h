/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:56:01 by aljulien          #+#    #+#             */
/*   Updated: 2024/07/30 14:16:46 by aljulien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <../libft/libft.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

#define EXIT_SHELL 42

extern int	g_ret;

enum e_EXPAND_CASES
{
	NO_VALUE,
	PRINT_DOLLAR,
	SINGLE_VALUE,
	VALUE_REST,
	S1_VALUE,
	S1_S2,
};

enum e_TOKENS
{
	CMD,
	ARG,
	REDIR_OP,
	PIPE,
};

enum e_REDIR_OPERATOR
{
	ZERO,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
};

typedef struct s_redir
{
	char			type;
	char			*fd;
	struct s_redir	*next;
	struct s_redir	*prev;
}	t_redir;

typedef struct s_pipe
{
	char			**arg;
	t_redir			*redir;
	t_redir			*redir_head;
	struct s_pipe	*next;
	struct s_pipe	*prev;
	int				ret_val;
}	t_pipe;

typedef struct s_argv
{
	int				node_index;
	char			*node;
	struct s_argv	*next;
	struct s_argv	*prev;
}	t_argv;

typedef struct s_line
{
	int				argc;
	t_argv			*argv;
	t_argv			*argv_head;
	t_pipe			*pipe;
	t_pipe			*pipe_head;
	char			**env;
	int				nm_arg;
}	t_line;

// =================================== PARSING ================================

int			main(int argc, char *argv[], char *exp[]);
bool		big_parse(t_line *line, char **input);
bool		lex(char *input, t_line *line);
bool		parse(t_line *line);

// W_SPACE
bool		is_white_space(char c);
size_t		skip_white_spaces(char **input);
// W_SPACE

// QUOTES
char		*find_matching_quote(char *str, char quote);
bool		even_quotes(char *str);
// QUOTES

// TOKENS_UTILS
bool		is_quote(char c, char quote);
char		*skip_quote_content(char *str, char quote);
bool		clean_surrounding_quotes(t_line *line);
// TOKENS_UTILS

// EXPANSION
bool		expand(t_line *line);
// EXPANSION

// PARSING_UTILS
size_t		ft_tablen(char **str);
// bool		extract_node(t_line *line);
// PARSING_UTILS

// STRUCT
t_line		*make_argv_node(char *input, size_t len, t_line *line);
size_t		count_argv_nodes(t_line *line);
// STRUCT

// REDIRECTIONS
bool		handle_redir(t_line *line, char	*first_redirection);
bool		process_redir(t_line *line, char redir_operator, \
	char *first_redirection);
char		is_redirection_operator(char *str);
char		skip_redirection_operator(char **str);
char		redirection_offset(char redir_operator);
// REDIRECTIONS

// =================================== PARSING ================================

//====================================BUILTINS=================================

int		ft_echo(char **arg);
void	ft_pwd(char **av);
void	ft_cd(char **av, char **env);
void	ft_env(char **env);
int		ft_exit (t_pipe *pipe);

//====================================BUILTINS=================================

// =================================== EXEC ================================

int		pipex(char **env, t_line *line);
int		execute_cmd(char **env, t_pipe *pipe);
char	*get_path(char *cmd, char **env, int i);
int		parse_builtin(t_pipe *pipe);
void	handle_exit_status_child(t_line *line, int status);

// =================================== EXEC ================================


//====================================OTHERS===================================
int	ft_putstr_fd(char *s, int fd);



#endif