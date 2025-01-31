/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:03:09 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/31 16:33:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

t_redir	*create_redir_node(t_redir_type type, char *file)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = ft_strdup(file);
	node->next = NULL;
	return (node);
}

void	add_redir_node(t_redir **head, t_redir *new_node)
{
	t_redir	*current;

	if (!*head)
	{
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

t_cmd	*create_cmd_node(void)
{
	t_cmd	*node;

	node = ft_calloc(sizeof(t_cmd), 1);
	if (!node)
		return (NULL);
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->args = NULL;
	node->input = NULL;
	node->output = NULL;
	node->next = NULL;
	return (node);
}