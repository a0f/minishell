/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/22 13:24:48 by showard       #+#    #+#                 */
/*   Updated: 2025/02/22 16:01:12 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>


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
		len_key = equals - envp[i];
		if (head == NULL)
			head = ft_mapnew(ft_substr(envp[i], 0, len_key),
					ft_strdup(&envp[i][len_key + 1]));
		else
		{
			node = ft_mapnew(ft_substr(envp[i], 0, len_key),
					ft_strdup(&envp[i][len_key + 1]));
			ft_mapadd_back(&head, node);
		}
		i++;
	}
	return (head);
}


void	print_map_export(t_map *lst)
{
	t_map	*current;

	current = lst;
	while (current != NULL)
	{
		printf("declare -x ");
		if (current->value == NULL)
			printf("%s\n", (char *)current->key);
		else
			printf("%s=%s\n", (char *)current->key, (char *)current->value);
		current = current->next;
	}
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


void	add_key(char *str, t_state *state, char *value, char *key)
{
	char	*temp;
	t_map	*node;

	node = map_find(state->env, match_key_str, key);
	if (value)
		node = ft_mapnew(key, ft_strdup(value + 1));
	else
	{
		temp = key;
		node = ft_mapnew(ft_strdup(str), NULL);
		free(key);
	}
	ft_mapadd_back(&state->env, node);
}


void	export(char *argv[], t_state *state)
{
	char	*key;
	char	*value;
	int		i;

	i = 0;
	if (argv[0] == NULL)
	{
		print_map_export(state->env);
		return ;
	}
	while (argv[i] != NULL)
	{
		value = ft_strchr(argv[i], '=');
		key = ft_substr(argv[i], 0, (value - argv[i]));
		if (map_find(state->env, match_key_str, key) && value)
			replace_value(state, value, key);
		else
			add_key(argv[i], state, value, key);
		i++;
	}
}