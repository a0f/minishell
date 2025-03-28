/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   debug_cmds.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/03 12:56:28 by showard       #+#    #+#                 */
/*   Updated: 2025/03/01 14:28:31 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

void	print_lst(t_list *lst)
{
	t_list	*current;

	current = lst;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}

void	debug_print_map(t_map *lst)
{
	t_map *current;

	current = lst;
	while (current != NULL)
	{
		printf("KEY: %s", (char *)current->key);
		printf("                               VALUE: %s\n",
			(char *)current->value);
		current = current->next;
	}
}
