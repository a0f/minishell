/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize4.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:22:12 by showard       #+#    #+#                 */
/*   Updated: 2025/04/24 16:39:35 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	run_tokenizer(t_state *state, t_tokens **tokens, char *cmd)
{
	while (*cmd)
	{
		while (*cmd == ' ' || *cmd == '\t')
			cmd++;
		if (*cmd)
		{
			cmd = tokenize_single(state->parser_pool, tokens, cmd);
			if (!cmd)
				return (1);
		}
	}
	return (0);
}
