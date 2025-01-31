/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 16:33:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

void	ft_free(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*key;

	i = 0;
	while (env[i])
	{
		key = ft_substr(env[i], 0, (char *)ft_strchr(env[i], '=') - env[i]);
		if (ft_strncmp(key, name, ft_strlen(key)) == 0)
		{
			free(key);
			return (ft_strchr(env[i], '=') + 1);
		}
		free(key);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	i = 0;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (cmd);
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free(paths);
	return (cmd);
}

void expander(t_cmd *cmd, t_shell *shell)
{
	int i;
	char *value;

	i = 0;
	if (!cmd->args)
		return ;
	if (ft_strncmp(cmd->args[0], "./", 2) != 0 && ft_strncmp(cmd->args[0], "../", 3) != 0)
		cmd->args[0] = resolve_path(cmd->args[0], shell->env);
    while (cmd->args[++i])
	{
        if (cmd->args[i][0] == '$')
		{
            value = get_env_value(cmd->args[i] + 1, shell->env);
            if (value)
			{
                free(cmd->args[i]);
                cmd->args[i] = ft_strdup(value);
            }
        }
    }
}
