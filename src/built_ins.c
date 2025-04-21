/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:10:02 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 15:43:20 by mwijnsma         ###   ########.fr       */
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

void	cd(t_state *state, char *path[])
{
	t_map	*pwd_node;
	char	*buffer;
	char	*temp;

	if (*path == NULL)
	{
		pwd_node = map_find(state->env, match_key_str, "HOME");
		chdir(pwd_node->value);
	}
	else
	{
		if (path[1] != NULL)
		{
			fprintf(stderr, "minishell: cd: too many arguments\n");
			state->last_exit_code = 1;
			return ;
		}
		if (chdir(*path) != 0)
		{
			state->last_exit_code = 1;
			perror("Error: ");
		}
		// error exit function needed?
	}
	pwd_node = map_find(state->env, match_key_str, "PWD");
	buffer = ft_calloc(PATH_MAX + 1, sizeof(char));
	if (buffer == NULL)
		error_func();
	temp = pwd_node->value;
	pwd_node->value = getcwd(buffer, PATH_MAX);
	free(temp);
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

void	unset(t_state *state, char *key)
{
	state->last_exit_code = 0;
	if (key == NULL)
		return;
	map_remove(&state->env, match_key_str, key);
}

void	exit_ms(t_state *state, char *argv[])
{
	int i;
	int exit_status;

	i = 0;
	if (argv[1] == NULL)
		(state_free(state), exit(0));
	if(argv[1][i] == '+' || argv[1][i] == '-')
		i++;
	while(argv[1][i] != '\0')
	{
		if (ft_isdigit(argv[1][i]) == 0)
		{
			fprintf(stderr, "exit\nminishell: exit: %s: numeric argument required\n", argv[1]);
			(state_free(state), exit(2));
		}
		i++;
	}
	if (argv[2] != NULL)
	{
		fprintf(stderr, "exit\nminishell: exit: too many arguments\n");
		state->last_exit_code = 1;
		return ;
	}
	exit_status = ft_atoi(argv[1]);
	(state_free(state), exit(exit_status));
}
