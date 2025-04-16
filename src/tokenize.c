/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 16:10:47 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/16 14:30:04 by showard       ########   odam.nl         */
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
	return (cmd);
}

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

char	*tokenize_word(t_pool *pool, t_tokens **tokens, char *cmd)
{
	t_tokens	*token;
	t_sb		*word;

	token = tokens_add(pool, tokens, TOKEN_WORD);
	word = sb_new(pool);
	while (*cmd && *cmd != ' ' && *cmd != '|' && *cmd != '\'' && *cmd != '"')
	{
		sb_append_char(word, *cmd);
		cmd++;
	}
	token->value = word->data;
	return (cmd);
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

char	*preprocess(t_state *state, char *cmd, bool in_heredoc, bool expand)
{
	bool	in_single;
	bool	in_double;
	t_sb	*out;
	size_t	i;
	t_sb	*var_sb;
	t_map *node;

	in_single = false;
	in_double = false;
	out = sb_new(state->parser_pool);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && !in_double && !in_heredoc)
		{
			in_single = !in_single;
			sb_append_char(out, cmd[i]);
			i++;
			continue ;
		}
		if (cmd[i] == '"' && !in_single && !in_heredoc)
		{
			in_double = !in_double;
			sb_append_char(out, cmd[i]);
			i++;
			continue ;
		}
		// hack if cmd[i + 1] to let $? through
		if (cmd[i] == '$' && !in_single && expand && cmd[i +1] != '?')
		{
			i++;
			var_sb = sb_new(state->parser_pool);
			while (cmd[i] && cmd[i] != '\'' && cmd[i] != '"' && cmd[i] != ' ' && cmd[i] != '\n')
			{
				sb_append_char(var_sb, cmd[i]);
				i++;
			}
			node = map_find(state->env, match_key_str, var_sb->data);
			if (node != NULL)
				sb_append(out, node->value);
		}
		else
		{
			sb_append_char(out, cmd[i]);
			i++;
		}
	}
	if (in_single || in_double)
	{
		printf("Unmatched quote\n");
		return (NULL);
	}
	return (out->data);
}

t_tokens	*tokenize(t_state *state, char *cmd, bool here_doc, bool expand)
{
	t_tokens	*tokens;

	tokens = NULL;
	cmd = preprocess(state, cmd, here_doc, expand);
	if (!cmd)
		return (NULL);
	if (here_doc)
	{
		tokens = tokens_new(state->parser_pool, TOKEN_WORD);
		tokens->value = pool_strdup(state->parser_pool, cmd);
		return (tokens);
	}
	while (*cmd)
	{
		while (*cmd == ' ')
			cmd++;
		if (*cmd)
		{
			cmd = tokenize_single(state->parser_pool, &tokens, cmd);
			if (!cmd)
				return (NULL);
		}
	}
	return (tokens);
}
