/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   built_ins.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 14:10:02 by showard       #+#    #+#                 */
/*   Updated: 2025/02/10 14:14:08 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <minishell.h>

void	error_func(void)
{
	printf("handle errors later lol\n");
}

void	echo(t_state *state, char *str)
{
	if (printf("%s", str) == -1)
		error_func();
}