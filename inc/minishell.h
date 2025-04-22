/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:13:14 by showard           #+#    #+#             */
/*   Updated: 2025/04/22 16:13:02 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "pool.h"
# include "sb.h"
# include "libft/libft.h"
# include "tokenize.h"
# include "parse.h"
# include "state.h"

typedef struct s_state	t_state;

extern int				g_signal;

void					*list_find(bool (*f)(void *));
void					print_lst(t_list *lst);
void					print_map_env(t_map *lst);
int						print_map_export(t_map *lst);
bool					match_key_str(void *node, void *key);
bool					match_key_int(void *node, void *key);
bool					match_key_long(void *node, void *key);
bool					match_key_size_t(void *node, void *key);
bool					match_key_float(void *node, void *key);
t_map					*map_find(t_map *head, bool (*f)(void *, void *),
							void *key);
void					map_remove(t_map **head, bool (*f)(void *, void *),
							void *key);
void					map_free(t_map **head);
void					echo(t_state *state, char *args[]);
void					cd(t_state *state, char *path[]);
bool					match_key_str_print(void *node, void *key);
void					pwd(t_state *state);
void					debug_print_map(t_map *lst);
void					env(t_state *state);
void					unset(t_state *state, char *key);
void					exit_ms(t_state *state, char *argv[]);
int						export(char *argv[], t_state *state);
t_map					*init_envp(t_state *state, t_map *head, char *envp[]);
void					write_stderr(char *str);
void					close_fds(void);
void					error_func(void);
void					replace_value(t_state *state, char *value, char *key);
bool					cmd_append_arg(t_pool *pool, t_cmd *cmd, char *arg);
t_input_file			*cmd_append_in_file(t_pool *pool, t_cmd *cmd);
t_output_file			*cmd_append_out_file(t_pool *pool, t_cmd *cmd);
t_cmd					*cmd_new(t_pool *pool);
bool					parse_gtgt(t_pool *pool, t_tokens **tokens, t_cmd *cmd);
bool					parse_ltlt(t_pool *pool, t_tokens **tokens, t_cmd *cmd);
bool					parse_gt(t_pool *pool, t_tokens **tokens, t_cmd *cmd);
bool					parse_lt(t_pool *pool, t_tokens **tokens, t_cmd *cmd);
bool					parse_word(t_pool *pool, t_tokens **tokens, t_cmd *cmd);
t_tokens				*tokens_add(t_pool *pool, t_tokens **tokens,
							t_token_type type);
char					*tokenize_single_quotes(t_pool *pool, t_tokens **tokens,
							char *cmd);
char					*tokenize_double_quotes(t_pool *pool, t_tokens **tokens,
							char *cmd);
bool					sb_append_int(t_sb *sb, int n);
t_tokens				*tokens_new(t_pool *pool, t_token_type type);
char					*tokenize_single(t_pool *pool, t_tokens **tokens,
							char *cmd);
void					sigint_interactive(int sig);

#endif // MINISHELL_H
