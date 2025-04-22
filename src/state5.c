/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state5.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:08:47 by showard       #+#    #+#                 */
/*   Updated: 2025/04/22 18:18:49 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <minishell.h>

int	heredoc_loop(t_state *state, bool expand, int fd, char *delimeter)
{
	char		*gnl_r;
	int			len;
	t_tokens	*tokens;
	int			res;

	write(1, "> ", 2);
	res = heredoc_getline(&gnl_r, delimeter, ft_strlen(delimeter));
	if (res == -1)
		return (-1);
	if (res == 1)
		return (1);
	tokens = tokenize(state, gnl_r, true, expand);
	if (!tokens)
		return (free(gnl_r), -1);
	while (tokens)
	{
		len = ft_strlen(tokens->value);
		if (write(fd, tokens->value, len) == -1)
		{
			(free(gnl_r), perror("write"), state_exit(state, 1));
		}
		tokens = tokens->next;
	}
	free(gnl_r);
	return (0);
}

bool	input_heredoc(t_state *state, char *delimeter, int *fd, bool expand)
{
	int	heredoc_check;

	g_signal = -2;
	while (true)
	{
		heredoc_check = heredoc_loop(state, expand, *fd, delimeter);
		if (heredoc_check == -1)
			return (false);
		else if (heredoc_check == 1)
			break ;
	}
	close(*fd);
	*fd = open("/tmp/tmp_heredoc", O_CREAT | O_RDWR, 0744);
	return (true);
}

bool	process_infile(t_state *state, t_cmd *cmd)
{
	t_input_file	*temp_in_file;
	int				fd;

	temp_in_file = cmd->in_files;
	while (temp_in_file)
	{
		if (temp_in_file->type == INPUT_HEREDOC)
		{
			fd = open("/tmp/tmp_heredoc", O_CREAT | O_RDWR | O_TRUNC, 0744);
			if (fd == -1)
				return (perror("open infile"), false);
			if (!input_heredoc(state, temp_in_file->value.s_heredoc.delimeter,
					&fd, temp_in_file->value.s_heredoc.expand))
				return (false);
		}
		else
		{
			fd = open(temp_in_file->value.path, O_RDONLY);
			if (fd == -1)
				return (perror("open infile"), false);
		}
		cmd->fds[READ_END] = fd;
		temp_in_file = temp_in_file->next;
	}
	return (true);
}

bool	process_outfile(t_cmd *cmd)
{
	t_output_file	*temp_out_file;
	int				fd;
	int				flags;

	temp_out_file = cmd->out_files;
	while (temp_out_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (temp_out_file->type == OUTPUT_TRUNCATE)
			flags |= O_TRUNC;
		else
			flags |= O_APPEND;
		fd = open(temp_out_file->path, flags, 0777);
		if (fd == -1)
		{
			perror("open outfile");
			return (false);
		}
		close(cmd->fds[WRITE_END]);
		cmd->fds[WRITE_END] = fd;
		temp_out_file = temp_out_file->next;
	}
	return (true);
}

void	get_previous_input(t_cmd *prev_cmd, t_cmd *cmd)
{
	if (prev_cmd->out_files == NULL)
		cmd->fds[READ_END] = cmd->pipe[READ_END];
	else
		cmd->fds[READ_END] = dup(STDIN_FILENO);
}
