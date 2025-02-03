/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_uint.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 11:23:25 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:07:30 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_printf_uint(unsigned int n, ssize_t *fcheck)
{
	char	c;
	int		counter;

	counter = 0;
	if (n > 9)
	{
		counter += ft_printf_uint(n / 10, fcheck);
	}
	c = (n % 10) + '0';
	ft_write(1, &c, 1, fcheck);
	counter++;
	return (counter);
}
