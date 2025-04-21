/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_key2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:11:00 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 16:45:34 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

bool	match_key_float(void *node, void *key)
{
	t_map	*map_node;

	map_node = (t_map *)node;
	if (*(float *)map_node->key == *(float *)key)
	{
		return (true);
	}
	return (false);
}
