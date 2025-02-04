/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: showard <showard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:11:00 by showard           #+#    #+#             */
/*   Updated: 2025/02/04 20:25:16 by showard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool match_key_str(void *node, void *key)
{
	t_map *map_node;
	
	map_node = (t_map *)node;
	if (ft_strcmp(map_node->key, (char *)key) == 0)
	{
		return (true);
	}
	return (false);
}

bool match_key_int(void *node, void *key)
{
	t_map *map_node;
	
	map_node = (t_map *)node;
	if (*(int*)map_node->key == *(int*)key)
	{
		return (true);
	}
	return (false);
}

bool match_key_long(void *node, void *key)
{
	t_map *map_node;
	
	map_node = (t_map *)node;
	if (*(long*)map_node->key == *(long*)key)
	{
		return (true);
	}
	return (false);
}

bool match_key_size_t(void *node, void *key)
{
	t_map *map_node;
	
	map_node = (t_map *)node;
	if (*(size_t*)map_node->key == *(size_t*)key)
	{
		return (true);
	}
	return (false);
}

bool match_key_float(void *node, void *key)
{
	t_map *map_node;
	
	map_node = (t_map *)node;
	if (*(float*)map_node->key == *(float*)key)
	{
		return (true);
	}
	return (false);
}