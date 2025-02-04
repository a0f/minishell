/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: showard <showard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:13:14 by showard           #+#    #+#             */
/*   Updated: 2025/02/04 20:40:12 by showard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <libft.h>

typedef struct {
	t_map *env;
	t_map *vars;
	pid_t running_command;
	int last_exit_code;
	char *last_line;
} t_state;

// void f() {
// 	t_list *list;

// 	list->as.var.name = "DOCKER_HOST";
// 	list->as.var.value = "abc";
// }


void 		*list_find(bool (*f)(void *));
void		print_lst(t_list *lst);
void		print_map(t_map *lst);
bool 		match_key_str(void *node, void *key);
bool 		match_key_int(void *node, void *key);
bool 		match_key_long(void *node, void *key);
bool 		match_key_size_t(void *node, void *key);
bool 		match_key_float(void *node, void *key);
void 		*map_find(t_map *head, bool (*f)(void *, void *), void *key);
void 		map_remove(t_map **head, bool (*f)(void *, void *), void *key);
void 		map_free(t_map **head);

#endif