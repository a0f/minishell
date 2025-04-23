/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state6.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:11:26 by showard       #+#    #+#                 */
/*   Updated: 2025/04/23 16:10:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	put_next_output(t_cmd *cmd)
{
	cmd->fds[WRITE_END] = cmd->pipe_into->pipe[WRITE_END];
}

bool	set_pipes(t_state *state, t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	t_cmd	*prev_cmd;

	temp_cmd = cmd;
	prev_cmd = NULL;
	while (temp_cmd)
	{
		temp_cmd->run = true;
		if (temp_cmd == cmd)
			temp_cmd->fds[READ_END] = dup(STDIN_FILENO);
		else
			get_previous_input(prev_cmd, temp_cmd);
		if (!temp_cmd->pipe_into)
			temp_cmd->fds[WRITE_END] = dup(STDOUT_FILENO);
		else
			put_next_output(temp_cmd);
		if (!process_infile(state, temp_cmd))
			temp_cmd->run = false;
		if (!process_outfile(temp_cmd))
			temp_cmd->run = false;
		prev_cmd = temp_cmd;
		temp_cmd = temp_cmd->pipe_into;
	}
	return (true);
}

void	restore_stds(int *original_stdin, int *original_stdout)
{
	dup2(*original_stdout, STDOUT_FILENO);
	close(*original_stdout);
	dup2(*original_stdin, STDIN_FILENO);
	close(*original_stdin);
}

void	link_cmd(t_state *state, t_cmd *cmd)
{
	if (dup2(cmd->fds[READ_END], STDIN_FILENO) == -1)
	{
		write_err("dup2 error\n");
		(state_free(state), exit(1));
	}
	close(cmd->fds[READ_END]);
	if (dup2(cmd->fds[WRITE_END], STDOUT_FILENO) == -1)
	{
		write_err("dup2 error\n");
		(state_free(state), exit(1));
	}
	close(cmd->fds[WRITE_END]);
}

void	init_cmd(t_state *state, t_cmd *cmd, int *og_stdin, int *og_stdout)
{
	*og_stdin = dup(STDIN_FILENO);
	*og_stdout = dup(STDOUT_FILENO);
	create_cmd_pipes(state, cmd);
	set_pipes(state, cmd);
	signal(SIGINT, SIG_IGN);
}
