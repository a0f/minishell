/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sb.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:14:29 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 17:13:11 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sb	*sb_new(t_pool *pool)
{
	t_sb	*sb;

	sb = pool_calloc(pool, sizeof(t_sb));
	if (!sb)
	{
		return (NULL);
	}
	sb->pool = pool;
	sb->data = pool_calloc(pool, 1);
	if (!sb->data)
	{
		return (NULL);
	}
	sb->count = 0;
	sb->capacity = 1;
	return (sb);
}

bool	sb_append(t_sb *sb, const char *str)
{
	size_t	len;

	len = strlen(str);
	sb->capacity += len;
	sb->data = pool_realloc(sb->pool, sb->data, sb->count, sb->capacity);
	if (!sb->data)
	{
		sb->capacity -= len;
		return (false);
	}
	ft_memcpy(sb->data + sb->count, str, len + 1);
	sb->count += len;
	return (true);
}

bool	sb_append_char(t_sb *sb, char c)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	return (sb_append(sb, str));
}

bool	sb_append_int(t_sb *sb, int n)
{
	bool	res;
	char	*str;

	str = ft_itoa(n);
	res = sb_append(sb, str);
	free(str);
	return (res);
}
