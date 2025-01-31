/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   planer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:01:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 15:22:02 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************
 *          INCLUDES          *
 ******************************/

# include "../ShellLibft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/******************************
 *          DEFINES           *
 ******************************/
# define SUCCESS 0
# define FAILURE 1
# define PROMPT "minishell-> "
# define READ_END 0
# define WRITE_END 1
#define MAX_TOKENS 1024

/******************************
 *          STRUCTS           *
 ******************************/

/* Redirection Types */
typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
}			t_redir_type;

typedef struct s_lexer
{
	char	*start;
	bool	in_quote;
	char	quote_char;
	int		token_count;
}			t_lexer;

/* Redirection Node */
typedef struct s_redir
{
	t_redir_type type;    // Type of redirection
	char *file;           // File or delimiter for heredoc
	struct s_redir *next; // Next redirection in the list
}			t_redir;

/* Command Node */
typedef struct s_cmd
{
	int in_fd;          // Input file descriptor
	int out_fd;         // Output file descriptor
	char **args;        // Command arguments (e.g., ["ls", "-l"])
	t_redir *input;     // Input redirections (e.g., < file.txt)
	t_redir *output;    // Output redirections (e.g., > out.txt)
	struct s_cmd *next; // Next command in the pipeline (e.g., cmd1 | cmd2)
}			t_cmd;

/* Shell State */
typedef struct s_shell
{
	char **env;          // Environment variables
	int exit_status;     // Exit status of the last command
	bool is_interactive; // Whether the shell is in interactive mode
	int signal_status;   // Global variable for signal handling
}			t_shell;

/******************************
 *      FUNCTION PROTOTYPES   *
 ******************************/

/* Main Loop */
void		minishell_loop(t_shell *shell);

/* Lexer */
char		**lexer(char *input);

/* Parser */
t_cmd		*parser(char **tokens);

/* Expander */
void		expander(t_cmd *cmd, t_shell *shell);

/* Executor */
void		executor(t_cmd *cmd, t_shell *shell);

/* Builtins */
void		execute_builtin(t_cmd *cmd);
int			is_builtin(char *cmd);
void		ft_echo(t_cmd *cmd);
void		ft_cd(t_cmd *cmd);
void		ft_pwd(void);
void		ft_export(t_cmd *cmd);
void		ft_unset(t_cmd *cmd);
void		ft_env(void);
void		ft_exit(t_cmd *cmd);

/* Redirections */
t_cmd		*create_cmd_node(void);
void		add_redir_node(t_redir **head, t_redir *new_node);
t_redir		*create_redir_node(t_redir_type type, char *file);
void		handle_pipeline(t_cmd **cmd, char **tokens, int i);
void		handle_redirections(t_cmd *cmd, char **tokens, int *i);
void		restore_redirections(t_cmd *cmd);
void		handle_dup(t_cmd *cmd);

/* Pipes */
void		execute_command(t_cmd *cmd);
void		execute_pipeline(t_cmd *cmd);
void		handle_pipes(t_cmd *cmd);

/* Signals */
void		setup_signals(void);
void		reset_signals(void);
void		ignore_signals(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);

/* Environment */
char		**copy_env(char **env);
char		*get_env_value(char *name, char **env);
void		set_env_value(char *key, char *value, t_shell *shell);

/* Utils */
void		ft_free(char **arr);
void		free_cmd(t_cmd *cmd);
void		free_tokens(char **tokens);
void		free_env(char **env);
void		print_error(char *msg, char *arg);

#endif
