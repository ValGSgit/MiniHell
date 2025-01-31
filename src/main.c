/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 16:37:04 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

int		g_signal_received = 0;

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	**tokens;
	t_cmd	*cmd;
	char	*prompt;

	prompt = "\033[0;92mMinishell-> \033[0m";  // Green color prompt
	while (1)
	{
		input = readline(prompt);
		if (!input)
		{
			write(1, "exit\n", 5);  // Handle Ctrl+D
			break;
		}
		if (*input)  // Add non-empty lines to history
			add_history(input);
		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			continue;
		}
		cmd = parser(tokens);
		if (!cmd)
		{
			free(input);
			free_tokens(tokens);
			continue;
		}
		expander(cmd, shell);
		execute_command(cmd);
		free_cmd(cmd);
		free_tokens(tokens);
		free(input);
	}
	rl_clear_history();  // Optional cleanup
}
// void	minishell_loop(t_shell *shell)
// {
// 	char	*input;
// 	size_t	len;
// 	char	**tokens;
// 	t_cmd	*cmd;

// 	input = NULL;
// 	len = 0;
// 	while (1)
// 	{
// 		printf("\033[0;92mMinishell-> \022");
// 		if (getline(&input, &len, stdin) == -1)
// 			break ; // Exit on EOF (Ctrl+D)
// 		// Remove newline character
// 		input[strlen(input) - 1] = '\0';
// 		// Lexer
// 		tokens = lexer(input);
// 		if (!tokens)
// 			continue ;
// 		// Parser
// 		cmd = parser(tokens);
// 		if (!cmd)
// 		{
// 			free_tokens(tokens);
// 			continue ;
// 		}
// 		// Expander
// 		expander(cmd, shell);
// 		// Executor
// 		execute_command(cmd);
// 		// executor(cmd, shell);
// 		// Cleanup
// 		free_cmd(cmd);
// 		free_tokens(tokens);
// 	}
// 	free(input);
// }

char **copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	if (*envp)
		shell.env = copy_env(envp);
	if (!shell.env)
		return (write(2, "_environment copy failed\n", 25), EXIT_FAILURE);
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	setup_signals();
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	return (shell.exit_status);
}