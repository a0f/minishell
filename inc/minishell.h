/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/21 15:13:14 by showard       #+#    #+#                 */
/*   Updated: 2025/04/23 16:14:43 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define READ_END 0
# define WRITE_END 1

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
void					write_err(char *str);
void					close_fds(void);
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
t_state					*state_new(void);
void					state_free(t_state *state);
void					state_exit(t_state *state, int code);
void					state_run_string(t_state *state, char *line);
void					sigint_interactive(int sig);
int						get_exit_status(pid_t pid);
char					*path_complete(t_pool *pool, char const *s1,
							char const *s2);
char					*find_valid_path(t_pool *pool, char **paths, char *cmd);
void					close_fds(void);
void					check_cmd(t_state *state, void **cmd);
void					state_execve_path(t_state *state, char **cmd,
							char *args[], t_map *path_node);
void					state_execve_child(t_state *state, char *cmd,
							char *args[], char *envp[]);
pid_t					state_execve(t_state *state, char *cmd, char **args,
							char **envp);
int						ft_mapsize(t_map *map);
char					*ft_strjoin_path(char const *s1, char const *s2);
char					**convert_env(t_map *state_env);
bool					is_builtin(t_cmd *cmd);
int						find_builtin(t_state *state, t_cmd *cmd);
void					create_cmd_pipes(t_state *state, t_cmd *cmd);
int						heredoc_getline(char **gnl_r, char *delimeter,
							int lim_len);
int						heredoc_loop(t_state *state, bool expand, int fd,
							char *delimeter);
bool					input_heredoc(t_state *state, char *delimeter, int fd,
							bool expand);
bool					process_infile(t_state *state, t_cmd *cmd);
bool					process_outfile(t_cmd *cmd);
void					get_previous_input(t_cmd *prev_cmd, t_cmd *cmd);
void					put_next_output(t_cmd *cmd);
bool					set_pipes(t_state *state, t_cmd *cmd);
void					restore_stds(int *original_stdin, int *original_stdout);
void					link_cmd(t_state *state, t_cmd *cmd);
void					init_cmd(t_state *state, t_cmd *cmd, int *og_stdin,
							int *og_stdout);
int						run_tokenizer(t_state *state, t_tokens **tokens,
							char *cmd);
void					free_null(void **ptr);

#endif
