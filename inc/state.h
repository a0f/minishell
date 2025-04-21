/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:39:12 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 16:17:06 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

# include "minishell.h"

typedef struct s_state	t_state;

struct s_state
{
	t_pool	*program_pool;
	t_pool	*parser_pool;
	t_map	*env;
	pid_t	running_command;
	int		last_exit_code;
};

t_state	*state_new(void);
void	state_free(t_state *state);
void	state_exit(t_state *state, int code);

void	state_run_string(t_state *state, char *line);
void	state_run_cmd(t_state *state, t_cmd *cmd);

#endif  // STATE_H
