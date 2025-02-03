/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_perc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/22 17:38:00 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:07:45 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_printf_perc(ssize_t *fcheck)
{
	ft_write(1, "%", 1, fcheck);
	return (1);
}
