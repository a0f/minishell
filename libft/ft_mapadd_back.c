/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_mapadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:18:02 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 16:15:05 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_mapadd_back(t_map **lst, t_map *new)
{
	t_map	*temp;

	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		temp = *lst;
		temp = ft_maplast(*lst);
		temp->next = new;
	}
}
