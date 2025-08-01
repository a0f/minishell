/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 14:19:42 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/23 16:10:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_err(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

void	run_minishell(t_state *state)
{
	char				*line;
	char				*parser_line;
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = sigint_interactive;
	sigaction(SIGINT, &act, NULL);
	line = readline("minishell> ");
	if (line == NULL)
		state_exit(state, 1);
	if (g_signal != -1)
	{
		if (g_signal == SIGINT)
			state->last_exit_code = 130;
		g_signal = -1;
	}
	add_history(line);
	parser_line = pool_strdup(state->parser_pool, line);
	free(line);
	if (!parser_line)
		state_exit(state, 1);
	state_run_string(state, parser_line);
}

int	main(int argc, char *argv[], char **envp)
{
	t_state	*state;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	g_signal = -1;
	state = state_new();
	if (!state)
		return (1);
	state->env = init_envp(state, state->env, envp);
	while (true)
	{
		run_minishell(state);
		pool_reset(state->parser_pool);
		pool_reset(state->program_pool);
	}
	return (0);
}
