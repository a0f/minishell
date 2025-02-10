/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:28:54 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 16:00:23 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_H
# define POOL_H

# include "minishell.h"

typedef struct s_pool
{
	void	**ptrs;
	size_t	count;
	size_t	capacity;
}	t_pool;

t_pool	*pool_new(void);
void	pool_free(t_pool *pool);
void	pool_reset(t_pool *pool);

void	*pool_calloc(t_pool *pool, size_t size);
void	*pool_realloc(t_pool *pool, void *ptr, size_t old_size,
			size_t new_size);

char	*pool_strdup(t_pool *pool, const char *s);

#endif  // POOL_H
