/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize2.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 16:10:47 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/24 16:40:16 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tokenize_pipe(t_pool *pool, t_tokens **tokens, char *cmd)
{
	tokens_add(pool, tokens, TOKEN_PIPE);
	return (cmd + 1);
}

char	*tokenize_lt(t_pool *pool, t_tokens **tokens, char *cmd)
{
	cmd++;
	if (*cmd == '<')
	{
		tokens_add(pool, tokens, TOKEN_LTLT);
		return (cmd + 1);
	}
	tokens_add(pool, tokens, TOKEN_LT);
	return (cmd);
}

char	*tokenize_gt(t_pool *pool, t_tokens **tokens, char *cmd)
{
	cmd++;
	if (*cmd == '>')
	{
		tokens_add(pool, tokens, TOKEN_GTGT);
		return (cmd + 1);
	}
	tokens_add(pool, tokens, TOKEN_GT);
	return (cmd);
}

char	*tokenize_word(t_pool *pool, t_tokens **tokens, char *c)
{
	t_tokens	*token;
	t_sb		*word;

	token = tokens_add(pool, tokens, TOKEN_WORD);
	word = sb_new(pool);
	while (*c && *c != ' ' && *c != '\t' && *c != '|'
		&& *c != '\'' && *c != '"')
	{
		sb_append_char(word, *c);
		c++;
	}
	token->value = word->data;
	token->ended_by_space = (*c == ' ' || *c == '\t');
	token->quoted = false;
	return (c);
}

char	*tokenize_single(t_pool *pool, t_tokens **tokens, char *cmd)
{
	char	c;

	c = *cmd;
	if (c == '\'')
		return (tokenize_single_quotes(pool, tokens, cmd));
	else if (c == '"')
		return (tokenize_double_quotes(pool, tokens, cmd));
	else if (c == '|')
		return (tokenize_pipe(pool, tokens, cmd));
	else if (c == '<')
		return (tokenize_lt(pool, tokens, cmd));
	else if (c == '>')
		return (tokenize_gt(pool, tokens, cmd));
	else
		return (tokenize_word(pool, tokens, cmd));
}
