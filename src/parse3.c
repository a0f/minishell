/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:18:37 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 18:42:23 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse(t_pool *pool, t_tokens *tokens)
{
	t_cmd			*first;
	t_cmd			*cmd;

	first = cmd_new(pool);
	cmd = first;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			cmd->pipe_into = cmd_new(pool);
			cmd = cmd->pipe_into;
		}
		if (tokens->type == TOKEN_WORD && !parse_word(pool, &tokens, cmd))
			return (NULL);
		if (tokens->type == TOKEN_LT && !parse_lt(pool, &tokens, cmd))
			return (NULL);
		if (tokens->type == TOKEN_GT && !parse_gt(pool, &tokens, cmd))
			return (NULL);
		if (tokens->type == TOKEN_LTLT && !parse_ltlt(pool, &tokens, cmd))
			return (NULL);
		if (tokens->type == TOKEN_GTGT && !parse_gtgt(pool, &tokens, cmd))
			return (NULL);
		tokens = tokens->next;
	}
	return (first);
}
