/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:18:00 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/17 16:16:03 by mwijnsma         ###   ########.fr       */
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

char	*pool_strjoin(t_pool *pool, const char *s1, const char *s2)
{
	char	*ptr;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	ptr = pool_calloc(pool, len1 + len2 + 1);
	if (!ptr)
	{
		return (NULL);
	}
	ft_memcpy(ptr, s1, len1);
	ft_memcpy(ptr + len1, s2, len2);
	return (ptr);
}
