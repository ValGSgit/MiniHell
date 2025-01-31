/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:33:20 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 16:33:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

extern char	**environ;

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	x;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		while (cmd->args[i])
		{
			x = 0;
			while (cmd->args[i][x])
			{
				if (cmd->args[i][x] != '\\' || (cmd->args[i][x] == ';' && cmd->args[i][x + 1] == ';'))
					printf("%c", cmd->args[i][x]);
				x++;
			}
			if (cmd->args[i + 1])
				printf(" ");
			i++;
		}
		// printf("%s", cmd->args[i]);
		// if (cmd->args[i + 1])
		// 	printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}

void	ft_cd(t_cmd *cmd)
{
	if (!cmd->args[1])
		chdir(getenv("HOME"));
	else
	{
		if (chdir(cmd->args[1]) != 0)
		{
			perror("cd");
		}
	}
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
	}
	else
	{
		perror("pwd");
	}
}
/* Helper function (put in separate file) */
int	add_new_env_var(char *arg)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (environ[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = environ[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	new_env[count + 1] = 0;
	environ = new_env;
	return (0);
}

// void	ft_export(t_cmd *cmd)
// {
// 	// Simplified export function
// 	if (cmd->args[1])
// 	{
// 		putenv(cmd->args[1]);
// 	}
// }

// void	ft_unset(t_cmd *cmd)
// {
// 	// Simplified unset function
// 	if (cmd->args[1])
// 	{
// 		unsetenv(cmd->args[1]);
// 	}
// }


void	ft_export(t_cmd *cmd)
{
	char	**env;
	char	*arg;
	char	*eq;
	int		i;

	if (!cmd->args[1])
	{
		env = environ;
		while (*env)
			printf("%s\n", *env++);
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		eq = strchr(arg, '=');
		env = environ;
		while (env && *env && !((eq && !strncmp(*env, arg, eq - arg)
						&& (*env)[eq - arg] == '=') || (!eq
					&& !strncmp(*env, arg, strlen(arg))
					&& (*env)[strlen(arg)] == '=')))
			env++;
		if (env && *env)
		{
			if (*env)
				free(*env);
			*env = ft_strdup(arg);
		}
		else if (add_new_env_var(arg))
			return ;
		i++;
	}
}

void	ft_unset(t_cmd *cmd)
{
	char	**e;
	char	*arg;
	int		i;
	int		j;

	if (!cmd->args[1])
		return ;
	i = 1;
	while (cmd->args[i])
	{
		arg = cmd->args[i++];
		e = environ;
		j = 0;
		while (e[j])
		{
			if (!strncmp(e[j], arg, strlen(arg)) && e[j][strlen(arg)] == '=')
			{
				free(e[j]);
				while (e[j])
				{
					e[j] = e[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
	}
}


void	ft_env(void)
{
	char	**environ;
	char	**env;

	environ = NULL;
	env = NULL;
	while (*env)
	{
		env = environ;
		printf("%s\n", *env);
		env++;
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_env();
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit(0);
}