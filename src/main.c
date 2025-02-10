/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:19:42 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 18:23:00 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_state	*state;
	char	*line;
	char	*parser_line;

	signal(SIGQUIT, SIG_IGN);
	state = state_new();
	if (!state)
		return (1);
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			state_exit(state, 1);
		}
		add_history(line);
		parser_line = pool_strdup(state->parser_pool, line);
		free(line);
		if (!parser_line)
			state_exit(state, 1);
		state_run_string(state, parser_line);
		pool_reset(state->parser_pool);
	}
	return (0);
}
