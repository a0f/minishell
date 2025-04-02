/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 16:08:45 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/02 16:40:48 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

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
	struct s_token	*next;
}	t_tokens;

t_tokens	*tokenize(t_state *state, char *cmd, bool here_doc, bool expand);

#endif  // TOKENIZE_H
