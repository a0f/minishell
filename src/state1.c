/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: showard <showard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:01:11 by showard           #+#    #+#             */
/*   Updated: 2025/04/23 10:36:10 by showard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	*state_new(void)
{
	t_state	*state;

	state = ft_calloc(1, sizeof(t_state));
	if (!state)
		return (NULL);
	state->program_pool = pool_new();
	if (!state->program_pool)
	{
		state_free(state);
		return (NULL);
	}
	state->parser_pool = pool_new();
	if (!state->parser_pool)
	{
		state_free(state);
		return (NULL);
	}
	state->static_pool = pool_new();
	if (!state->static_pool)
	{
		state_free(state);
		return (NULL);
	}
	state->env = NULL;
	return (state);
}

void	state_free(t_state *state)
{
	if (state->program_pool)
	{
		pool_free(state->program_pool);
	}
	if (state->parser_pool)
	{
		pool_free(state->parser_pool);
	}
	if (state->static_pool)
	{
		pool_free(state->static_pool);
	}
	if (state->env)
	{
		map_free(&state->env);
	}
	close_fds();
	rl_clear_history();
	free(state);
}

void	state_exit(t_state *state, int code)
{
	state_free(state);
	exit(code);
}

void	state_run_string(t_state *state, char *line)
{
	t_tokens	*tokens;
	t_cmd		*cmd;

	tokens = tokenize(state, line, false, true);
	if (!tokens)
	{
		return ;
	}
	cmd = parse(state->parser_pool, tokens);
	if (!cmd)
	{
		return ;
	}
	state_run_cmd(state, cmd);
}

void	sigint_interactive(int sig)
{
	bool	was_in_heredoc;

	was_in_heredoc = g_signal == -2;
	g_signal = sig;
	if (sig == SIGINT)
	{
		printf("\n");
		if (!was_in_heredoc)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}
