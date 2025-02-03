/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/27 12:19:29 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 21:10:33 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if ((unsigned char)*s1 != (unsigned char)*s2)
		{
			return ((unsigned char)*s1 - (unsigned char)*s2);
		}
		s1++;
		s2++;
	}
	return (0);
}

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

bool match_key(void *node, void *key)
{
    t_map *map_node = (t_map *)node;
    return strcmp(map_node->key, (char *)key) == 0;
}

void *map_find(t_map *head, bool (*f)(void *, void *), void *key)
{
    t_map *current = head;

    while (current != NULL)
    {
        if (f(current, key))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		*state;
	t_map		*current;

	argc = 0;
	argv = NULL;
	state = ft_calloc(1, sizeof(t_state));
	if (state == NULL)
		return (1);
		
	state->env = init_envp(state->env, envp);
	current = state->env;
    // Find a node with the key "TARGET_KEY"
    t_map *found_node;
	found_node = (t_map *)map_find(state->env, match_key, "PATH");
    if (found_node != NULL)
    {
        printf("key: %s\nvalue: %s\n", found_node->key, found_node->value);
    }
    else
    {
        printf("Node not found\n");
    }
	// print_map(state->env);
	while (1)
	{
		state->last_line = readline("test> ");
		if (state->last_line == NULL)
			break;
		add_history(state->last_line);
		free(state->last_line);
	}
	free(state->last_line);
	rl_clear_history();
	// free_and_exit(state);
}
