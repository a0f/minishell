/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:24:48 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 18:28:49 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

void	*tern(bool cond, void *a, void *b)
{
	if (cond)
		return (a);
	return (b);
}

void	get_kv(t_state *state, char *entry, char **key, char **value)
{
	char	*equals;
	int		len_key;

	equals = ft_strchr(entry, '=');
	len_key = (int)(uintptr_t)tern(equals, (void *)(equals - entry),
			(void *)ft_strlen(entry));
	*key = pool_substr(state->static_pool, entry, 0, len_key);
	if (!*key)
		state_exit(state, 1);
	*value = pool_strdup(state->static_pool, &entry[len_key + 1]);
	if (!*value)
		state_exit(state, 1);
}

t_map	*init_envp(t_state *state, t_map *head, char *envp[])
{
	int		i;
	char	*key;
	char	*value;
	t_map	*node;

	i = 0;
	while (envp[i] != NULL)
	{
		get_kv(state, envp[i], &key, &value);
		if (head == NULL)
			head = ft_mapnew(key, tern(ft_strchr(envp[i], '='), value, NULL));
		else
		{
			node = ft_mapnew(key, tern(ft_strchr(envp[i], '='), value, NULL));
			if (!node)
				state_exit(state, 1);
			ft_mapadd_back(&head, node);
		}
		if (!head)
			state_exit(state, 1);
		i++;
	}
	return (head);
}

int	print_map_export(t_map *lst)
{
	t_map	*current;

	current = lst;
	while (current != NULL)
	{
		printf("declare -x ");
		if (current->value == NULL)
			printf("%s\n", (char *)current->key);
		else
			printf("%s=\"%s\"\n", (char *)current->key, (char *)current->value);
		current = current->next;
	}
	return (0);
}

void	replace_value(t_state *state, char *value, char *key)
{
	t_map	*node;

	node = map_find(state->env, match_key_str, key);
	node->value = pool_strdup(state->static_pool, value);
	if (!node->value)
		state_exit(state, 1);
}
