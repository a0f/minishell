/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstclear.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 14:37:15 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:29:31 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;
	t_list	*last_node;

	temp = *lst;
	while (temp)
	{
		del(temp->content);
		last_node = temp;
		temp = temp->next;
		free(last_node);
	}
	*lst = NULL;
}
