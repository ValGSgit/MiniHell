/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:43:36 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 16:38:50 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"


char	**build_args_array(char **tokens, int argc)
{
	char	**args;
	int		i;

	args = ft_calloc(argc + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = -1;
	while (++i < argc)
		args[i] = ft_strdup(tokens[i]);
	return (args);
}

bool	is_metacharacter(char *token)
{
	return (ft_strcmp(token, "|") == 0
		|| ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<<") == 0);
}

int	count_args(char **tokens)
{
	int	count;

	count = 0;
	while (tokens[count] && !is_metacharacter(tokens[count]))
		count++;
	return (count);
}

void	handle_redirections(t_cmd *cmd, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], "<") == 0)
		add_redir_node(&cmd->input, create_redir_node(REDIR_IN, tokens[++(*i)]));
	else if (ft_strcmp(tokens[*i], ">") == 0)
		add_redir_node(&cmd->output, create_redir_node(REDIR_OUT, tokens[++(*i)]));
	else if (ft_strcmp(tokens[*i], ">>") == 0)
		add_redir_node(&cmd->output, create_redir_node(REDIR_APPEND, tokens[++(*i)]));
	else if (ft_strcmp(tokens[*i], "<<") == 0)
		add_redir_node(&cmd->input, create_redir_node(REDIR_HEREDOC, tokens[++(*i)]));
}

void	handle_pipeline(t_cmd **cmd, char **tokens, int i)
{
	t_cmd	*pipeline;

	if (ft_strcmp(tokens[i], "|") == 0)
	{
		pipeline = create_cmd_node();
		if (pipeline)
		{
			(*cmd)->next = pipeline;
			*cmd = pipeline;
		}
	}
}

int	is_redirection(char *token)
{
	return (ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0
		|| ft_strcmp(token, "<<") == 0);
}

t_cmd	*parser(char **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	int		i;
	int		argc;

	head = NULL;
	current = NULL;
	i = -1;
	while (tokens[++i])
	{
		if (!head && (head = create_cmd_node()))
			current = head;
		else if (!current)
			break ;
		if (is_redirection(tokens[i]))
			handle_redirections(current, tokens, &i);
		else if (ft_strcmp(tokens[i], "|") == 0)
			handle_pipeline(&current, tokens, i);
		else
		{
			argc = count_args(&tokens[i]);
			current->args = build_args_array(&tokens[i], argc);
			i += argc - 1;
		}
	}
	return (head);
}