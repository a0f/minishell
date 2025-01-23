/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 15:56:09 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 13:59:38 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*n_list;
	t_list	*n_node;
	void	*content;

	n_list = NULL;
	while (lst)
	{
		content = f(lst->content);
		if (content == NULL)
		{
			ft_lstclear(&n_list, del);
			return (NULL);
		}
		n_node = ft_lstnew(content);
		if (n_node == NULL)
		{
			del(content);
			ft_lstclear(&n_list, del);
			return (NULL);
		}
		ft_lstadd_back(&n_list, n_node);
		lst = lst->next;
	}
	return (n_list);
}
