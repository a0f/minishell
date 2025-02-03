/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/21 15:13:14 by showard       #+#    #+#                 */
/*   Updated: 2025/02/01 13:50:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stddef.h>
#include <sys/types.h>

typedef struct s_state{
	t_list *env;
	pid_t running_command;
	int last_exit_code;
} t_state;

#endif