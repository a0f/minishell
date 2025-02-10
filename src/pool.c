/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:18:00 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 16:10:13 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pool	*pool_new(void)
{
	t_pool	*pool;

	pool = ft_calloc(1, sizeof(t_pool));
	if (!pool)
	{
		return (NULL);
	}
	pool->capacity = 1;
	pool->ptrs = ft_calloc(pool->capacity, sizeof(void *));
	if (!pool->ptrs)
	{
		free(pool);
		return (NULL);
	}
	return (pool);
}

void	pool_free(t_pool *pool)
{
	size_t	i;

	i = 0;
	while (i < pool->count)
	{
		free(pool->ptrs[i]);
		i++;
	}
	free(pool->ptrs);
	free(pool);
}

void	pool_reset(t_pool *pool)
{
	size_t	i;

	i = 0;
	while (i < pool->count)
	{
		free(pool->ptrs[i]);
		i++;
	}
	pool->count = 0;
}

void	*pool_calloc(t_pool *pool, size_t size)
{
	void	*ptr;
	void	**new_ptrs;

	ptr = ft_calloc(1, size);
	if (!ptr)
	{
		return (NULL);
	}
	if (pool->count == pool->capacity)
	{
		pool->capacity *= 2;
		new_ptrs = ft_realloc(pool->ptrs, pool->count * sizeof(void *),
				pool->capacity * sizeof(void *));
		if (!new_ptrs)
		{
			pool->capacity /= 2;
			free(ptr);
			return (NULL);
		}
		pool->ptrs = new_ptrs;
	}
	pool->ptrs[pool->count++] = ptr;
	return (ptr);
}

void	*pool_realloc(t_pool *pool, void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	i;

	new_ptr = ft_realloc(ptr, old_size, new_size);
	if (!new_ptr)
	{
		return (NULL);
	}
	i = 0;
	while (i < pool->count)
	{
		if (pool->ptrs[i] == ptr)
		{
			pool->ptrs[i] = new_ptr;
			break ;
		}
		i++;
	}
	return (new_ptr);
}
