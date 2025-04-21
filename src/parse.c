/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:18:37 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 16:12:46 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_new(t_pool *pool)
{
	t_cmd *cmd = pool_calloc(pool, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = pool_calloc(pool, sizeof(char *));
	if (!cmd->args)
		return (NULL);
	return (cmd);
}

bool	cmd_append_arg(t_pool *pool, t_cmd *cmd, char *arg)
{
	size_t len = 0;
	while (cmd->args[len])
		len++;
	cmd->args = pool_realloc(pool, cmd->args, len * sizeof(char *), (len + 2) * sizeof(char *));
	if (!cmd->args)
		return (false);
	cmd->args[len] = arg;
	cmd->args[len + 1] = NULL;
	return (true);
}

t_input_file	*cmd_append_in_file(t_pool *pool, t_cmd *cmd)
{
	t_input_file	*file;

	file = cmd->in_files;
	while (file && file->next)
		file = file->next;
	if (!file)
	{
		cmd->in_files = pool_calloc(pool, sizeof(t_input_file));
		if (!cmd->in_files)
			return (NULL);
		file = cmd->in_files;
	}
	else
	{
		file->next = pool_calloc(pool, sizeof(t_input_file));
		if (!file->next)
			return (NULL);
		file = file->next;
	}
	return (file);
}

t_output_file	*cmd_append_out_file(t_pool *pool, t_cmd *cmd)
{
	t_output_file	*file;

	file = cmd->out_files;
	while (file && file->next)
		file = file->next;
	if (!file)
	{
		cmd->out_files = pool_calloc(pool, sizeof(t_output_file));
		if (!cmd->out_files)
			return (NULL);
		file = cmd->out_files;
	}
	else
	{
		file->next = pool_calloc(pool, sizeof(t_output_file));
		if (!file->next)
			return (NULL);
		file = file->next;
	}
	file->next = NULL;
	return (file);
}

t_cmd *parse(t_pool *pool, t_tokens *tokens) {
	t_cmd *first = cmd_new(pool);
	t_cmd *cmd = first;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD) {
			if (!cmd->program) {
				cmd->program = pool_strdup(pool, tokens->value);
				if (!cmd->program) {
					return (NULL);
				}
				if (!cmd_append_arg(pool, cmd, pool_strdup(pool, tokens->value)))
					return (NULL);
			} else {
				if (!cmd_append_arg(pool, cmd, pool_strdup(pool, tokens->value))) {
					return (NULL);
				}
			}
		} else if (tokens->type == TOKEN_PIPE) {
			cmd->pipe_into = cmd_new(pool);
			cmd = cmd->pipe_into;
		} else if (tokens->type == TOKEN_LT) {
			t_input_file *in_file = cmd_append_in_file(pool, cmd);
			if (!in_file) {
				return (NULL);
			}
			in_file->type = INPUT_FILE;
			tokens = tokens->next;
			if (!tokens || tokens->type != TOKEN_WORD)
			{
				printf("Expected input file\n");
				return (NULL);
			}
			in_file->value.path = pool_strdup(pool, tokens->value);
		} else if (tokens->type == TOKEN_GT) {
			t_output_file *out_file = cmd_append_out_file(pool, cmd);
			if (!out_file) {
				return (NULL);
			}
			out_file->type = OUTPUT_TRUNCATE;
			tokens = tokens->next;
			if (!tokens || tokens->type != TOKEN_WORD)
			{
				printf("Expected output file\n");
				return (NULL);
			}
			out_file->path = pool_strdup(pool, tokens->value);
		} else if (tokens->type == TOKEN_LTLT) {
			t_input_file *in_file = cmd_append_in_file(pool, cmd);
			if (!in_file) {
				return (NULL);
			}
			in_file->type = INPUT_HEREDOC;
			tokens = tokens->next;
			if (!tokens || tokens->type != TOKEN_WORD)
			{
				printf("Expected heredoc delimiter\n");
				return (NULL);
			}
			in_file->value.heredoc.delimeter = pool_strdup(pool, tokens->value);
			in_file->value.heredoc.expand = !tokens->quoted;
		} else if (tokens->type == TOKEN_GTGT) {
			t_output_file *out_file = cmd_append_out_file(pool, cmd);
			if (!out_file) {
				return (NULL);
			}
			out_file->type = OUTPUT_APPEND;
			tokens = tokens->next;
			if (!tokens || tokens->type != TOKEN_WORD)
			{
				printf("Expected output file\n");
				return (NULL);
			}
			out_file->path = pool_strdup(pool, tokens->value);
		} else {
			printf("Unknown token type\n");
			return (NULL);
		}
		tokens = tokens->next;
	}
	return (first);
}

void cmd_dump(t_cmd *cmd) {
	fprintf(stderr, "program: %s\n", cmd->program);
	fprintf(stderr, "input fd: %d\n", cmd->fds[0]);
	fprintf(stderr, "output fd: %d\n", cmd->fds[1]);
	fprintf(stderr, "args: [");
	for (size_t i = 0; cmd->args[i]; i++) {
		fprintf(stderr, "%s, ", cmd->args[i]);
	}
	fprintf(stderr, "]\n");
	if (cmd->in_files) {
		fprintf(stderr, "in_files: [");
		t_input_file *file = cmd->in_files;
		while (file) {
			if (file->type == INPUT_FILE) {
				fprintf(stderr, "file %s, ", file->value.path);
			} else {
				fprintf(stderr, "heredoc %s, ", file->value.heredoc.delimeter);
			}
			file = file->next;
		}
		fprintf(stderr, "]\n");
	}
	if (cmd->out_files) {
		fprintf(stderr, "out_files: [");
		t_output_file *file = cmd->out_files;
		while (file) {
			if (file->type == OUTPUT_APPEND) {
				fprintf(stderr, "append %s, ", file->path);
			} else {
				fprintf(stderr, "truncate %s, ", file->path);
			}
			file = file->next;
		}
		fprintf(stderr, "]\n");
	}
	if (cmd->pipe_into) {
		fprintf(stderr, "-- pipe into --\n");
		cmd_dump(cmd->pipe_into);
	}
}
