/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:19:29 by showard           #+#    #+#             */
/*   Updated: 2025/02/10 15:12:03 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

t_map *init_envp(t_map *head, char *envp[])
{
	int		i;
	int		len_key;
	t_map 	*node;
	char	*equals;

	i = 0;
	while (envp[i] != NULL)
	{
		equals = ft_strchr(envp[i], '=');
		len_key = equals - envp[i];
		// if (equals == NULL)
		// 	return (NULL);
		if (head == NULL)
			head = ft_mapnew(ft_substr(envp[i], 0, len_key), ft_strdup(&envp[i][len_key + 1]));
		else
		{
			node = ft_mapnew(ft_substr(envp[i], 0, len_key), ft_strdup(&envp[i][len_key + 1]));
			ft_mapadd_back(&head, node);
		}
		i++;
	}
	return (head);
}


// void	ft_lstfree(t_env **lst)
// {
// 	t_env	*temp;
// 	t_env	*last_node;

// 	temp = *lst;
// 	while (temp)
// 	{
// 		last_node = temp;
// 		temp = temp->next;
// 		free(last_node->value);
// 		free(last_node);
// 	}
// 	*lst = NULL;
// }

// void free_and_exit(t_state *state)
// {
// 	ft_lfree(&state->env);
// 	free(state);
// 	state = NULL;
// }

// void *state_calloc(t_state *state, size_t size) {
// 	addr = ft_calloc();
// 	add_to_list(state->some_list, addr)
// 	return addr;
// }

// void state_free(t_state *state, void *addr) {
// 	remove_from_list(find_in_list(state->some_list, addr);
// }

void *add_mlist(t_list **head, size_t sizeof_data, size_t count)
{
	t_list *new_node;
	
    void *content = malloc(sizeof_data * count);
    if (content == NULL)
        return NULL;
    new_node = ft_lstnew(content);
	if (new_node == NULL)
	{
		free(content);
		return (NULL);
	}
	ft_lstadd_back(head, new_node);
    return content;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		*state;

	(void)argc;
    (void)argv;
	state = ft_calloc(1, sizeof(t_state));
	if (state == NULL)
		return (1);
		
	state->env = init_envp(state->env, envp);
    t_map *found_node;
	found_node = (t_map *)map_find(state->env, match_key_str, "PATH");
    if (found_node != NULL)
    {
        printf("key: %s\nvalue: %s\n", found_node->key, found_node->value);
    }
    else
    {
        printf("Node not found\n");
    }
	// while (1)
	// {
	// 	state->last_line = readline("test> ");
	// 	if (state->last_line == NULL)
	// 		break;
	// 	add_history(state->last_line);
	// 	free(state->last_line);
	// }
	echo(state, )
	free(state->last_line);
	rl_clear_history();
	// free_and_exit(state);
}
