/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 14:19:42 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/02/28 15:47:59 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_state	*state;
	char	*line;
	char	*parser_line;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	state = state_new();
	state->env = init_envp(state->env, envp);
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
