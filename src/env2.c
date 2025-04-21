/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:24:48 by showard           #+#    #+#             */
/*   Updated: 2025/04/21 16:41:12 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "state.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

int	check_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '\0')
	{
		(write_stderr("minishell: export: '"), write_stderr(arg));
		write_stderr("': not a valid identifier\n");
		return (1);
	}
	if (ft_isalpha(arg[0]) == 0 && arg[0] != '_')
	{
		(write_stderr("minishell: export: '"), write_stderr(arg));
		write_stderr("': not a valid identifier\n");
		return (1);
	}
	while (arg[i++])
	{
		if ((!ft_isalnum(arg[i - 1]) && arg[i - 1] != '_' && arg[i - 1] != '='))
		{
			(write_stderr("minishell: export: '"), write_stderr(arg));
			write_stderr("': not a valid identifier\n");
			return (1);
		}
	}
	return (0);
}

int	export(char *argv[], t_state *state)
{
	char	*key;
	int		i;

	i = 0;
	if (argv[0] == NULL)
		return (print_map_export(state->env));
	while (argv[i] != NULL)
	{
		if (ft_strchr(argv[i], '='))
			key = ft_substr(argv[i], 0, (ft_strchr(argv[i], '=') - argv[i]));
		else
			key = ft_strdup(argv[i]);
		if (check_valid_identifier(key) != 0)
		{
			state->last_exit_code = 1;
			return ;
		}
		if (map_find(state->env, match_key_str, key) && ft_strchr(argv[i], '='))
			replace_value(state, ft_strchr(argv[i], '='), key);
		else
			add_key(state, ft_strchr(argv[i], '='), key);
		i++;
	}
}
