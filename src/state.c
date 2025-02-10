/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:41:34 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/02/10 18:44:51 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	*state_new(void)
{
	t_state	*state;

	state = ft_calloc(1, sizeof(t_state));
	if (!state)
	{
		return (NULL);
	}
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
	// todo: init other state vars
	return (state);
}

void	state_free(t_state *state)
{
	// todo: free other state vars
	if (state->program_pool)
	{
		pool_free(state->program_pool);
	}
	if (state->parser_pool)
	{
		pool_free(state->parser_pool);
	}
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

	tokens = tokenize(state->parser_pool, line);
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
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	get_exit_status(pid_t pid)
{
	int	status;

	wait4(pid, &status, 0, NULL);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (WEXITSTATUS(status));
}

pid_t	state_execve(t_state *state, char *path, char **argv, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		// todo: free everything we don't need in the child
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(path, argv, envp);
	}
	state->running_command = pid;
	return (pid);
}

void	state_run_cmd(t_state *state, t_cmd *cmd)
{
	// todo: properly implement
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = state_execve(state, cmd->program, cmd->args, NULL);
	// run second command
	// run third command
	// pid = run_lsat_Command();  // using state_execve
	state->last_exit_code = get_exit_status(pid);
	signal(SIGINT, sigint_interactive);
}
