/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   built_ins2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 14:10:02 by showard       #+#    #+#                 */
/*   Updated: 2025/04/23 19:07:17 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "state.h"

void	unset(t_state *state, char *key)
{
	state->last_exit_code = 0;
	if (key == NULL)
		return ;
	map_remove(&state->env, match_key_str, key);
}

void	exit_ms(t_state *state, char *argv[])
{
	int	i;
	int	exit_status;

	i = 0;
	if (argv[1] == NULL)
		(state_free(state), exit(0));
	if (argv[1][i] == '+' || argv[1][i] == '-')
		i++;
	while (argv[1][i++] != '\0')
	{
		if (!ft_isdigit(argv[1][i - 1]))
		{
			(write_err("exit\nminishell: exit: "), write_err(argv[1]));
			write_err(": numeric argument required\n");
			(state_free(state), exit(2));
		}
	}
	if (argv[2] != NULL)
	{
		write_err("exit\nminishell: exit: too many arguments\n");
		state->last_exit_code = 1;
		return ;
	}
	exit_status = ft_atoi(argv[1]);
	(state_free(state), exit(exit_status));
}

void	cd_home(t_state *state, t_map **pwd_node)
{
	*pwd_node = map_find(state->env, match_key_str, "HOME");
	if (!*pwd_node || !(*pwd_node)->value)
	{
		write_err("minishell: cd: HOME not set\n");
		return ;
	}
	chdir((*pwd_node)->value);
}

bool	cd_path(t_state *state, char *path[])
{
	if (path[1] != NULL)
	{
		write_err("minishell: cd: too many arguments\n");
		state->last_exit_code = 1;
		return (false);
	}
	if (chdir(*path) != 0)
	{
		state->last_exit_code = 1;
		perror("Error: ");
	}
	return (true);
}

void	cd(t_state *state, char *path[])
{
	t_map	*pwd_node;
	char	*buffer;

	if (*path == NULL)
		cd_home(state, &pwd_node);
	else
	{
		if (!cd_path(state, path))
			return ;
	}
	pwd_node = map_find(state->env, match_key_str, "PWD");
	buffer = pool_calloc(state->static_pool, PATH_MAX + 1);
	if (buffer == NULL)
		state_exit(state, 1);
	if (!pwd_node)
	{
		pwd_node = ft_mapnew("PWD", NULL);
		if (!pwd_node)
			state_exit(state, 1);
		ft_mapadd_back(&state->env, pwd_node);
	}
	pwd_node->value = getcwd(buffer, PATH_MAX);
}
