/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sb.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:30:36 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 15:34:26 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SB_H
# define SB_H

# include "minishell.h"

typedef struct s_sb
{
	char	*data;
	size_t	count;
	size_t	capacity;
	t_pool	*pool;
}	t_sb;

t_sb	*sb_new(t_pool *pool);

bool	sb_append(t_sb *sb, const char *str);
bool	sb_append_char(t_sb *sb, char c);

#endif  // SB_H
