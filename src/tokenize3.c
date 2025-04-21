/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:10:47 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 17:32:56 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	preprocess_expand(t_state *state, size_t *i, char *cmd, t_sb *out)
{
	t_sb	*var_sb;

	(*i)++;
	var_sb = sb_new(state->parser_pool);
	while (cmd[*i] && cmd[*i] != '\'' && cmd[*i] != '"' && cmd[*i] != ' '
		&& cmd[*i] != '\n')
	{
		sb_append_char(var_sb, cmd[*i]);
		(*i)++;
		if (var_sb->data[0] == '?')
			break ;
	}
	if (ft_strcmp(var_sb->data, "?") == 0)
		sb_append_int(out, state->last_exit_code);
	if (map_find(state->env, match_key_str, var_sb->data) != NULL)
		sb_append(out, map_find(state->env, match_key_str,
				var_sb->data)->value);
}

void	preprocess_loop(t_args *args)
{
	if (((args->cmd[args->i] == '\'' && !args->in_double)
			|| (args->cmd[args->i] == '"' && !args->in_single))
		&& !args->in_heredoc)
	{
		if (args->cmd[args->i] == '\'')
			args->in_single = !args->in_single;
		if (args->cmd[args->i] == '"')
			args->in_double = !args->in_double;
		sb_append_char(args->out, args->cmd[args->i++]);
		return ;
	}
	if (args->cmd[args->i] == '$' && !args->in_single && args->expand
		&& (ft_isalpha(args->cmd[args->i + 1]) || args->cmd[args->i + 1] == '_'
			|| args->cmd[args->i + 1] == '?'))
		preprocess_expand(args->state, &args->i, args->cmd, args->out);
	else
		sb_append_char(args->out, args->cmd[args->i++]);
}

char	*preprocess(t_state *state, char *cmd, bool in_heredoc, bool expand)
{
	t_args	args;

	args.state = state;
	args.cmd = cmd;
	args.in_heredoc = in_heredoc;
	args.expand = expand;
	args.in_single = false;
	args.in_double = false;
	args.out = sb_new(state->parser_pool);
	args.i = 0;
	while (args.cmd[args.i])
		preprocess_loop(&args);
	if (args.in_single || args.in_double)
	{
		write_stderr("Unmatched quote\n");
		return (NULL);
	}
	return (args.out->data);
}

t_tokens	*merge_tokens(t_state *state, t_tokens *tokens)
{
	t_tokens	*current;
	t_tokens	*next;

	current = tokens;
	while (current && current->next)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && next->type == TOKEN_WORD
			&& !current->ended_by_space)
		{
			current->value = pool_strjoin(state->parser_pool, current->value,
					next->value);
			current->next = next->next;
			current->ended_by_space = next->ended_by_space;
			current->quoted |= next->quoted;
		}
		else
			current = current->next;
	}
	return (tokens);
}

t_tokens	*tokenize(t_state *state, char *cmd, bool here_doc, bool expand)
{
	static t_tokens	*tokens = NULL;

	cmd = preprocess(state, cmd, here_doc, expand);
	if (!cmd)
		return (NULL);
	if (here_doc)
	{
		tokens = tokens_new(state->parser_pool, TOKEN_WORD);
		tokens->value = pool_strdup(state->parser_pool, cmd);
		if (!tokens->value)
			return (NULL);
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
	return (merge_tokens(state, tokens));
}
