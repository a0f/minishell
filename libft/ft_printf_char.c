/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_char.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/22 16:33:25 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:07:54 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_printf_char(char c, ssize_t *fcheck)
{
	ft_write(1, &c, 1, fcheck);
	return (1);
}
