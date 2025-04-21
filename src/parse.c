/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:18:37 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 16:47:13 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_new(t_pool *pool)
{
	t_cmd	*cmd;

	cmd = pool_calloc(pool, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = pool_calloc(pool, sizeof(char *));
	if (!cmd->args)
		return (NULL);
	return (cmd);
}

bool	cmd_append_arg(t_pool *pool, t_cmd *cmd, char *arg)
{
	size_t	len;

	len = 0;
	while (cmd->args[len])
		len++;
	cmd->args = pool_realloc(pool, cmd->args, len * sizeof(char *), (len + 2)
			* sizeof(char *));
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
