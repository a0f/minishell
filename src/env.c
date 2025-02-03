/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/27 12:19:29 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 12:28:04 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

t_list *init_envp(t_list *head, char *envp[])
{
	int		i;
	t_list *node;

	i = 0;
	while (envp[i] != NULL)
	{
		if (head == NULL)
			head = ft_lstnew(ft_strdup(envp[i]));
		else
		{
			node = ft_lstnew(ft_strdup(envp[i]));
			ft_lstadd_back(&head, node);
		}
		i++;
	}
	return (head);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state *state;
	t_list *current;

	argc = 0;
	argv = NULL;
	state = ft_calloc(1, sizeof(state));
	if (state == NULL)
		return (1);
		
	state->env = init_envp(state->env, envp);
	current = state->env;
	while (current != NULL)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}