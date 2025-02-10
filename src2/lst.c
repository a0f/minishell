/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lst.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/03 14:21:45 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 16:23:06 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>


// typedef struct s_map {
// 	int	(*compare)(void *a, void *b);
// } t_map;

// t_map *map_new(int	(*compare)(void *a, void *b)) {
// 	t_map *map = malloc(sizeof(t_map));

// 	map->compare(left, right);
// }

// int var_compare(t_var *a, t_var *b) {
// 	return strcmp(a->name, b->name);
// }


// t_vars	*vars_lstnew(void *content)
// {
// 	t_vars	*n;

// 	n = malloc(sizeof(t_vars));
// 	if (n == NULL)
// 	{
// 		return (NULL);
// 	}
// 	n->next = NULL;
// 	return (n);
// }

// t_vars	*vars_lstlast(t_vars *lst)
// {
// 	if (!lst)
// 	{
// 		return (NULL);
// 	}
// 	while (lst->next)
// 	{
// 		lst = lst->next;
// 	}
// 	return (lst);
// }
// void	vars_lstadd_back(t_vars **lst, t_vars *new)
// {
// 	t_vars	*temp;

// 	if (*lst == NULL)
// 	{
// 		*lst = new;
// 	}
// 	else
// 	{
// 		temp = *lst;
// 		temp = ft_lstlast(*lst);
// 		temp->next = new;
// 	}
// }

// int	ft_lstsize(t_vars *lst)
// {
// 	int	counter;

// 	counter = 0;
// 	while (lst != NULL)
// 	{
// 		counter++;
// 		lst = lst->next;
// 	}
// 	return (counter);
// }