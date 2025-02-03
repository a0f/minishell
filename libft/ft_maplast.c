/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_maplast.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:01:01 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 16:14:11 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_map	*ft_maplast(t_map *lst)
{
	if (!lst)
	{
		return (NULL);
	}
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
