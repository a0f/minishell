/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/21 15:13:14 by showard       #+#    #+#                 */
/*   Updated: 2025/02/03 20:28:22 by showard       ########   odam.nl         */
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

#endif