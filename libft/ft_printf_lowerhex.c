/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_lowerhex.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 11:04:30 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:07:48 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_printf_lowerhex(unsigned int hex_long, ssize_t *fcheck)
{
	const char		*hex_map;
	char			buffer[17];
	int				i;
	int				count;

	hex_map = "0123456789abcdef";
	i = 0;
	if (hex_long == 0)
		buffer[i++] = '0';
	while (hex_long != 0)
	{
		buffer[i++] = hex_map[hex_long % 16];
		hex_long /= 16;
	}
	buffer[i] = '\0';
	count = i;
	while (i > 0)
	{
		ft_write(1, &buffer[--i], 1, fcheck);
	}
	return (count);
}
