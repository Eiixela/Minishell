/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljulien <aljulien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:59:32 by aljulien          #+#    #+#             */
/*   Updated: 2024/09/08 19:20:39 by aljulien         ###   ########.fr       */
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
# include <limits.h>

# define EXIT_SHELL 42

extern int	g_ret;

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

typedef struct s_env
{
	char			*env;
	struct s_env	*prev;
	int				is_exported;
	struct s_env	*next;
}	t_env;

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
	bool			true_pipe;
	char			*node;
	struct s_argv	*next;
	struct s_argv	*prev;
}	t_argv;

typedef struct s_line
{
	size_t			skipped_char;
	int				argc;
	t_argv			*argv;
	t_argv			*argv_head;
	t_pipe			*pipe;
	t_pipe			*pipe_head;
	t_env			*env;
	int				nm_arg;
	int				exit_status;
}	t_line;

typedef struct s_io_fds
{
	int	input_fd;
	int	output_fd;
}	t_io_fds;

typedef struct s_process_info
{
	t_env	*env;
	t_line	*line;
	char	*str;
	int		pipe_fd;
}	t_process_info;

// =================================== PARSING ================================

int			main(int argc, char *argv[], char *exp[]);
int			init_env(t_env **env, char **envp);
void		initialize(t_line *line, int *status);
int			setup_environment(t_env **env, char **envp, t_line *line);
void		handle_exit(t_line *line, t_env *env);
void		update_status(int *status, t_line *line);
char		*big_parse(t_line *line, char **input, int status);
char		*expand(char *input, t_line *line, char *value, short squote_mode);
bool		lex(char *input, t_line *line);
bool		parse(t_line *line, int status);

// W_SPACE
bool		is_white_space(char c);
size_t		skip_white_spaces(char **input);

// QUOTES
char		*find_matching_quote(char *str, char quote);
bool		even_quotes(char *str);

// TOKENS_UTILS
bool		is_quote(char c, char quote);
char		*skip_quote_content(char *str, char quote);
bool		clean_surrounding_quotes(t_line *line);
char		*which_token(char *input, t_line *line);

// LEXING_UTILS
char		*fill_argv(char *input, t_line *line, size_t token_len);
t_line		*make_argv_node(char *input, size_t len, t_line *line);
t_line		*first_argv(char *input, size_t len, t_line *line);

// EXPAND_UTILS
size_t		_strlen(char const *str);
bool		is_valid_varname(char c);
char		*_strdup(const char *s);
char		*towards_expand(char *dollar_index, t_line *line, char *str_head, \
	char *value);
bool		is_env_var_format(char *input, short squote_mode);
bool		is_exit_status_format(char *input, short squote_mode);
char		*get_value(char *dollar_index, t_line *line, char	*name, \
	int *name_len);
void		free_s1_value_rest(char *s1, char *value, char *rest);
void		handle_exit_status_exec(t_line *line, int status);
char		*get_env_value(t_line *line, char *name);
void		turn_extra_dollar_negative(char	**s1);
char		*handle_env_var(char *input, t_line *line, char **str_head, \
	char *value);
void		quote_mode(char *input, short *squote_mode, short *dquote_mode);

// PARSING_UTILS
size_t		ft_tablen(char **str);
t_line		*init_line_pipe(t_line *line, int status);
void		get_value_back(char *str);

// STRUCT
t_line		*make_argv_node(char *input, size_t len, t_line *line);
size_t		count_argv_nodes(t_line *line);

// REDIRECTIONS
bool		handle_redir(t_line *line, char	*first_redirection);
bool		process_redir(t_line *line, char redir_operator, \
	char *first_redirection);
char		is_redirection_operator(char *str);
char		skip_redirection_operator(char **str);
char		redirection_offset(char redir_operator);
void		clean_redir(char *str);
void		dirtier_redir(char *str);

// CLEANUP
void		cleanup(t_line *line);
void		free_env(t_env *env);
void		free_argv(t_argv *argv);
void		free_pipe(t_pipe *pipe);
// =================================== PARSING ================================

//======================================EXEC===================================

//BUILTINS
//ECH0
int			ft_echo(char **arg);

//PWD
int			ft_pwd(t_env *env);

//CD
int			ft_cd(t_env *env, t_line *line);
char		*check_len(char	*path, t_env *env);
int			pwds(t_env *env, char *path);
int			check_directory(char *var, char *path);
char		*prep_path(char *var, char *path);
char		*split_wsep(char *str, char sep);
char		*expand_tilde(const char *arg, t_env *env);

//ENV
int			ft_env(t_env *env, t_pipe *pipe);
t_env		*env_newnode(char *data);
void		env_addback(t_env **env, t_env *node);
size_t		env_len(t_env *env);
void		create_env(char **envp, t_env **env);
char		**arenvlst(t_env *env);

//EXIT
int			ft_exit(t_pipe *pipe, t_line *line);
int			print_error_message(char *s1, char *s2, char *s3);

//UNSET
int			ft_unset(t_line **line, t_env *env);
int			is_valid_identifier(const char *str);
size_t		env_len_unset(t_env *env);

//EXPORT
int			ft_export(t_pipe **pipe, t_env *env);
int			sort_env(t_env	*env);
char		*cut_string(const char *input_string, char cut_char);
void		free_for_export(int index, char **s, char **temp);
int			check_arg(char *var);
char		*find_var_env(t_env *env, char *var);

//BUILTINS UTILS
void		*exprt_inenv(t_env **env, char *data);
int			parse_builtin(t_pipe *pipe);
size_t		ft_arrlen(char **arr);

//ERROR
void		*errjoin(int error_code, char *error_message);
void		*print_error(int error_code, char *error_message);
int			verror(char *s1, char *s2, char *s3);

//FREE
void		env_freelst(t_env **env);
void		free_all_tab(char **s_cmd, char **allpath);
void		free_double_tab(char **s);
void		cleanup_exec(t_line *line);

//SIGNALS
char		*send_eof(char *line);
void		siglisten(void);
void		sigend(void);
void		sighandler(int sig);
void		handle_exit_status_child(t_line *line, int status, \
	int quit_message_printed);

//HEREDOC
char		*expand_variables(const char *input, t_env *env);
char		*gen_filename(int fn);
int			process_expanded_line(int fd, char *expanded_line);
int			write_to_heredoc(int fd, const char *content);
int			process_heredocs(t_line *line, t_env *env);
int			open_and_dup_heredoc(char *fd_name);
int			read_heredoc_lines(int fd, char *delimiter, t_env *env);
int			process_heredoc_line(int fd, char *line, t_env *env);

//REDIRECTIONS
int			redirection_in_pipe(t_pipe *pipe, int *saved_output);
int			redir_heredoc(t_pipe *pipe, t_env *env);
int			handle_single_heredoc(char *delimiter, const char *temp_file, \
	t_env *env);
int			handle_redirection(t_pipe *pipe);
int			handle_heredoc(const char *filename, t_line *line, t_env *env);
int			handle_output_redirection(char *filename, t_redir *redir, \
	t_line *line, t_env *env);
int			handle_input_redirection(char *filename, t_line *line, t_env *env);
void		setup_child_io(t_io_fds *fds, int pipe_fd);
int			setup_io(t_io_fds *fds);
void		apply_redirection(int *fd, int target_fd, t_line *line, t_env *env);
void		handle_redir_type(t_redir *redir, t_io_fds *fds, \
	t_line *line, t_env *env);
void		while_redir(t_line *line, t_pipe *pipe, t_env *env);

//EXECUTING
int			pipex(t_env *env, t_line *line, int *status, char *str);
int			execute_cmd(t_env *env, t_pipe *pipe, t_line *line, char *str);
char		*get_path(t_pipe *pipe, char **env, int i);
int			parse_and_execute_solo_builtins(t_env *env, t_line *line, \
	int saved_output);
int			create_process(t_process_info *info, t_io_fds *fds, t_pipe *pipe);
int			call_childs(t_env *env, t_line *line, char *str, pid_t last_pid);
int			execute_builtins(t_env *env, t_pipe *pipe, t_line *line);
int			handle_redirection(t_pipe *pipe);

//UTILS
char		*back_to_positive(char *s);
void		initialize_variables(t_line *line, int *status);
int			initialize_environment(t_env **env, char **envp);

// EXECVE_UTILS
int			check_command_exists(char **arg);
int			handle_builtin_execution(t_env *env, t_pipe *pipe, t_line *line);
int			check_file_permissions(char *cmd);
void		cleanup_resources(char **env_now, char *str, char *path);
int			execute_external_command(t_env *env, t_pipe *pipe, char **env_now, \
	char *str);
void		error_and_free_for_exec(t_env *env, t_line *line, char *path, \
	int i);

// =================================== EXEC ================================

#endif
