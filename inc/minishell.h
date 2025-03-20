/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/21 15:13:14 by showard       #+#    #+#                 */
/*   Updated: 2025/02/22 13:27:48 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "pool.h"
# include "sb.h"
# include "tokenize.h"
# include "parse.h"
# include "state.h"

void 		*list_find(bool (*f)(void *));
void		print_lst(t_list *lst);
void		print_map_env(t_map *lst);
void		print_map_export(t_map *lst);
bool 		match_key_str(void *node, void *key);
bool 		match_key_int(void *node, void *key);
bool 		match_key_long(void *node, void *key);
bool 		match_key_size_t(void *node, void *key);
bool 		match_key_float(void *node, void *key);
void 		*map_find(t_map *head, bool (*f)(void *, void *), void *key);
void 		map_remove(t_map **head, bool (*f)(void *, void *), void *key);
void 		map_free(t_map **head);
void		echo(t_state *state, char *args[]);
void		cd(t_state *state, char *path[]);
bool 		match_key_str_print(void *node, void *key);
void		pwd(t_state *state);
void		debug_print_map(t_map *lst);
void		env(t_state *state);
void		unset(t_state *state, char *key);
void		exit_ms(t_state *state, char *argv[]);
void		export(char *argv[], t_state *state);
t_map		*init_envp(t_map *head, char *envp[]);


#endif  // MINISHELL_H
