/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 15:41:34 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/22 17:14:57 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int		g_signal;

void	run_cmd(t_state *state, t_cmd *cmd, int *non_builtin)
{
	char	**envp;

	link_cmd(state, cmd);
	envp = convert_env(state->env);
	if (envp == NULL)
		(state_free(state), exit(1));
	if (is_builtin(cmd))
		free_2d(envp);
	if (cmd->run)
	{
		if (!find_builtin(state, cmd))
		{
			*non_builtin = 1;
			cmd->pid = state_execve(state, cmd->program, cmd->args, envp);
		}
		else
			cmd->pid = -1;
	}
	if (!is_builtin(cmd))
		free_2d(envp);
}

void	wait_all_cmds(t_cmd *cmd)
{
	t_cmd	*temp_cmd;

	temp_cmd = cmd;
	while (temp_cmd)
	{
		if (temp_cmd->pid != -1 && temp_cmd->run)
		{
			waitpid(temp_cmd->pid, NULL, 0);
		}
		temp_cmd = temp_cmd->pipe_into;
	}
}

void	state_run_cmd(t_state *state, t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	int		original_stdin;
	int		original_stdout;
	int		non_builtin;

	init_cmd(state, cmd, &original_stdin, &original_stdout);
	temp_cmd = cmd;
	while (temp_cmd)
	{
		non_builtin = 0;
		run_cmd(state, temp_cmd, &non_builtin);
		if (!temp_cmd->pipe_into)
			break ;
		temp_cmd = temp_cmd->pipe_into;
	}
	if (!temp_cmd->run)
		state->last_exit_code = 1;
	else if (non_builtin == 1)
		state->last_exit_code = get_exit_status(temp_cmd->pid);
	if (state->last_exit_code == 130)
		printf("\n");
	restore_stds(&original_stdin, &original_stdout);
	close_fds();
	wait_all_cmds(cmd);
}
