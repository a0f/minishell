/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   built_ins.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 14:10:02 by showard       #+#    #+#                 */
/*   Updated: 2025/04/23 19:07:55 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "state.h"

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
	char	buffer[PATH_MAX + 1];

	(void)state;
	signal(SIGPIPE, SIG_IGN);
	printf("%s\n", getcwd(buffer, PATH_MAX));
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
