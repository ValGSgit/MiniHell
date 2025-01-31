/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:34:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 15:07:48 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

void	execute_command(t_cmd *cmd)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_dup(cmd);
		execvp(cmd->args[0], cmd->args);
		perror("execvp");
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
		restore_redirections(cmd);
	}
}

void	execute_pipeline(t_cmd *cmd)
{
	int		pipe_fd[2];
	int		prev_pipe_in;
	pid_t	pid;

	prev_pipe_in = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			pipe(pipe_fd);
			cmd->out_fd = pipe_fd[1];
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_pipe_in != 0)
			{
				dup2(prev_pipe_in, 0);
				close(prev_pipe_in);
			}
			if (cmd->out_fd != 1)
			{
				dup2(cmd->out_fd, 1);
				close(cmd->out_fd);
			}
			handle_dup(cmd);
			execvp(cmd->args[0], cmd->args);
			perror("execvp");
			exit(1);
		}
		else
		{
			if (prev_pipe_in != 0)
				close(prev_pipe_in);
			if (cmd->out_fd != 1)
				close(cmd->out_fd);
			prev_pipe_in = pipe_fd[0];
			cmd = cmd->next;
		}
	}
	waitpid(pid, NULL, 0);
}

void	handle_dup(t_cmd *cmd)
{
	if (cmd->in_fd != 0)
		dup2(cmd->in_fd, 0);
	if (cmd->out_fd != 1)
		dup2(cmd->out_fd, 1);
}

void	restore_redirections(t_cmd *cmd)
{
	if (cmd->in_fd != 0)
		close(cmd->in_fd);
	if (cmd->out_fd != 1)
		close(cmd->out_fd);
}