/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:08:45 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/04/21 18:35:30 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

typedef struct s_state	t_state;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_LT,
	TOKEN_GT,
	TOKEN_LTLT,
	TOKEN_GTGT,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	bool			ended_by_space;
	bool			quoted;
	struct s_token	*next;
}	t_tokens;

typedef struct s_args
{
	t_state	*state;
	char	*cmd;
	bool	in_heredoc;
	bool	expand;
	bool	in_single;
	bool	in_double;
	size_t	i;
	t_sb	*out;
}	t_args;

t_tokens	*tokenize(t_state *state, char *cmd, bool here_doc, bool expand);

#endif  // TOKENIZE_H
