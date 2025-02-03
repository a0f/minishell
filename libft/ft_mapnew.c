/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_mapnew.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 11:14:55 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 16:07:30 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_map	*ft_mapnew(void *key, void *value)
{
	t_map	*n;

	n = malloc(sizeof(t_map));
	if (n == NULL)
	{
		return (NULL);
	}
	n->key = key;
	n->value = value;
	n->next = NULL;
	return (n);
}
