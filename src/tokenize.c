/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:10:47 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 17:07:28 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*tokens_new(t_pool *pool, t_token_type type)
{
	t_tokens	*token;

	token = pool_calloc(pool, sizeof(t_tokens));
	if (!token)
	{
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

t_tokens	*tokens_last(t_tokens *tokens)
{
	while (tokens && tokens->next)
	{
		tokens = tokens->next;
	}
	return (tokens);
}

t_tokens	*tokens_add(t_pool *pool, t_tokens **tokens, t_token_type type)
{
	t_tokens	*last;

	if (!*tokens)
	{
		*tokens = tokens_new(pool, type);
		return (*tokens);
	}
	else
	{
		last = tokens_last(*tokens);
		last->next = tokens_new(pool, type);
		return (last->next);
	}
}

char	*tokenize_single_quotes(t_pool *pool, t_tokens **tokens, char *cmd)
{
	t_tokens	*token;
	t_sb		*word;

	token = tokens_add(pool, tokens, TOKEN_WORD);
	if (!token)
		return (NULL);
	word = sb_new(pool);
	if (!word)
		return (NULL);
	cmd++;
	while (*cmd && *cmd != '\'')
	{
		sb_append_char(word, *cmd);
		cmd++;
	}
	if (!*cmd)
	{
		printf("Unmatched quote\n");
		return (NULL);
	}
	cmd++;
	token->value = word->data;
	token->ended_by_space = *cmd == ' ';
	token->quoted = true;
	return (cmd);
}

char	*tokenize_double_quotes(t_pool *pool, t_tokens **tokens, char *cmd)
{
	t_tokens	*token;
	t_sb		*word;

	token = tokens_add(pool, tokens, TOKEN_WORD);
	word = sb_new(pool);
	cmd++;
	while (*cmd && *cmd != '"')
	{
		sb_append_char(word, *cmd);
		cmd++;
	}
	if (!*cmd)
	{
		printf("Unmatched quote\n");
		return (NULL);
	}
	cmd++;
	token->value = word->data;
	token->ended_by_space = *cmd == ' ';
	token->quoted = true;
	return (cmd);
}
