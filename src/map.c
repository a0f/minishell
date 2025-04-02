/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/04 20:09:55 by showard       #+#    #+#                 */
/*   Updated: 2025/04/02 16:16:40 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_map	*map_find(t_map *head, bool (*f)(void *, void *), void *key)
{
	t_map	*current;

	current = head;
	while (current != NULL)
	{
		if (f(current, key))
		{
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

void	map_remove(t_map **head, bool (*f)(void *, void *), void *key)
{
	t_map	*current;
	t_map	*prev;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (f(current, key))
		{
			if (prev == NULL)
			{
				*head = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	map_free(t_map **head)
{
	t_map *temp;
	t_map *last_node;

	temp = *head;
	while (temp != NULL)
	{
		last_node = temp;
		temp = temp->next;
		free(last_node->key);
		free(last_node->value);
		free(last_node);
	}
	*head = NULL;
}