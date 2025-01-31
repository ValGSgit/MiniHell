/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:04 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 14:14:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	free(redir->file);
	free_redir(redir->next);
	free(redir);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	for (int i = 0; cmd->args[i]; i++)
	{
		free(cmd->args[i]);
	}
	free(cmd->args);
	free_redir(cmd->input);
	free_redir(cmd->output);
	free_cmd(cmd->next);
	free(cmd);
}

void	free_tokens(char **tokens)
{
	for (int i = 0; tokens[i]; i++)
	{
		free(tokens[i]);
	}
	free(tokens);
}

void	free_env(char **env)
{
	for (int i = 0; env[i]; i++)
	{
		free(env[i]);
	}
	free(env);
}