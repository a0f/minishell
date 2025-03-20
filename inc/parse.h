/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:06:42 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/03/20 14:21:56 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"

typedef enum s_input_type
{
	INPUT_FILE,
	INPUT_HEREDOC,
}	t_input_type;

typedef union s_input_value
{
	char	*path;
	char	*delimeter;
}	t_input_value;

typedef struct s_input_file
{
	t_input_type		type;
	t_input_value		value;
	struct s_input_file	*next;
}	t_input_file;

typedef enum s_output_type
{
	OUTPUT_APPEND,
	OUTPUT_TRUNCATE,
}	t_output_type;

typedef struct s_output_file
{
	t_output_type			type;
	char					*path;
	struct s_output_file	*next; 
}	t_output_file;

typedef struct s_cmd
{
	char			*program;
	char			**args;
	t_input_file	*in_files;
	t_output_file	*out_files;
	struct s_cmd	*pipe_into;
	int				fds[2];
	int				pipe[2];
	pid_t			pid;
}	t_cmd;

t_cmd	*parse(t_pool *pool, t_tokens *tokens);
void	cmd_dump(t_cmd *cmd);  // todo: remove this

#endif  // PARSE_H