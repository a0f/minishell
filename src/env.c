/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:24:48 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 16:41:00 by mwijnsma         ###   ########.fr       */
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

t_map	*init_envp(t_map *head, char *envp[])
{
	int		i;
	int		len_key;
	t_map	*node;
	char	*equals;

	i = 0;
	while (envp[i] != NULL)
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
			len_key = equals - envp[i];
		else
			len_key = ft_strlen(envp[i]);
		if (head == NULL)
			head = ft_mapnew(ft_substr(envp[i], 0, len_key),
					tern(equals, ft_strdup(&envp[i][len_key + 1]), NULL));
		else
		{
			node = ft_mapnew(ft_substr(envp[i], 0, len_key),
					tern(equals, ft_strdup(&envp[i][len_key + 1]), NULL));
			ft_mapadd_back(&head, node);
		}
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
	char	*temp;
	t_map	*node;

	node = map_find(state->env, match_key_str, key);
	temp = node->value;
	node->value = ft_strdup(value + 1);
	(free(key), free(temp));
}

void	add_key(t_state *state, char *value, char *key)
{
	t_map	*node;

	if (value)
	{
		node = ft_mapnew(key, ft_strdup(value + 1));
	}
	else
	{
		node = ft_mapnew(key, NULL);
	}
	ft_mapadd_back(&state->env, node);
}
