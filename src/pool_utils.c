/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:18:00 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 16:10:20 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pool_strdup(t_pool *pool, const char *s)
{
	char	*ptr;
	size_t	len;

	len = ft_strlen(s);
	ptr = pool_calloc(pool, len + 1);
	if (!ptr)
	{
		return (NULL);
	}
	ft_memcpy(ptr, s, len);
	return (ptr);
}
