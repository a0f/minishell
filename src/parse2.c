/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 16:18:37 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/23 16:10:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_word(t_pool *pool, t_tokens **tokens, t_cmd *cmd)
{
	if (!cmd->program)
	{
		cmd->program = pool_strdup(pool, (*tokens)->value);
		if (!cmd->program)
		{
			return (false);
		}
		if (!cmd_append_arg(pool, cmd, pool_strdup(pool,
					(*tokens)->value)))
			return (false);
	}
	else
	{
		if (!cmd_append_arg(pool, cmd, pool_strdup(pool,
					(*tokens)->value)))
		{
			return (false);
		}
	}
	return (true);
}

bool	parse_lt(t_pool *pool, t_tokens **tokens, t_cmd *cmd)
{
	t_input_file	*in_file;

	in_file = cmd_append_in_file(pool, cmd);
	if (!in_file)
	{
		return (false);
	}
	in_file->type = INPUT_FILE;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		write_err("Expected input file\n");
		return (false);
	}
	in_file->value.path = pool_strdup(pool, (*tokens)->value);
	return (true);
}

bool	parse_gt(t_pool *pool, t_tokens **tokens, t_cmd *cmd)
{
	t_output_file	*out_file;

	out_file = cmd_append_out_file(pool, cmd);
	if (!out_file)
	{
		return (false);
	}
	out_file->type = OUTPUT_TRUNCATE;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		write_err("Expected output file\n");
		return (false);
	}
	out_file->path = pool_strdup(pool, (*tokens)->value);
	return (true);
}

bool	parse_ltlt(t_pool *pool, t_tokens **tokens, t_cmd *cmd)
{
	t_input_file	*in_file;

	in_file = cmd_append_in_file(pool, cmd);
	if (!in_file)
	{
		return (false);
	}
	in_file->type = INPUT_HEREDOC;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		write_err("Expected heredoc delimiter\n");
		return (false);
	}
	in_file->value.s_heredoc.delimeter = pool_strdup(pool,
			(*tokens)->value);
	in_file->value.s_heredoc.expand = !(*tokens)->quoted;
	return (true);
}

bool	parse_gtgt(t_pool *pool, t_tokens **tokens, t_cmd *cmd)
{
	t_output_file	*out_file;

	out_file = cmd_append_out_file(pool, cmd);
	if (!out_file)
	{
		return (false);
	}
	out_file->type = OUTPUT_APPEND;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		write_err("Expected output file\n");
		return (false);
	}
	out_file->path = pool_strdup(pool, (*tokens)->value);
	return (true);
}
