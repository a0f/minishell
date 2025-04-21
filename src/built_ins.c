/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:10:02 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 16:25:18 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "state.h"

void	error_func(void)
{
	printf("handle errors later lol\n");
}

void	echo(t_state *state, char *args[])
{
	bool	print_newline;

	signal(SIGPIPE, SIG_IGN);
	print_newline = true;
	if (*args && ft_strcmp("-n", *args) == 0)
	{
		print_newline = false;
		args++;
	}
	while (*args != NULL)
	{
		if (args[1] == NULL)
			printf("%s", *args);
		else
			printf("%s ", *args);
		args++;
	}
	if (print_newline)
		printf("\n");
	state->last_exit_code = 0;
	signal(SIGPIPE, SIG_DFL);
}

void	pwd(t_state *state)
{
	t_map	*pwd_node;

	signal(SIGPIPE, SIG_IGN);
	pwd_node = map_find(state->env, match_key_str, "PWD");
	printf("%s\n", pwd_node->value);
	signal(SIGPIPE, SIG_DFL);
}

void	env(t_state *state)
{
	t_map	*current;

	signal(SIGPIPE, SIG_IGN);
	current = state->env;
	while (current != NULL)
	{
		if (current->value != NULL)
			printf("%s=%s\n", (char *)current->key, (char *)current->value);
		current = current->next;
	}
	signal(SIGPIPE, SIG_DFL);
}
