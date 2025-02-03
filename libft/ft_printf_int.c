/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_int.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/22 16:20:36 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:07:51 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_printf_int(int n, ssize_t *fcheck)
{
	char	c;
	int		counter;

	counter = 0;
	if (n == -2147483648)
	{
		ft_write(1, "-2147483648", 11, fcheck);
		return (11);
	}
	if (n < 0)
	{
		ft_write(1, "-", 1, fcheck);
		n *= -1;
		counter++;
	}
	if (n > 9)
	{
		counter += ft_printf_int(n / 10, fcheck);
	}
	c = (n % 10) + '0';
	ft_write(1, &c, 1, fcheck);
	counter++;
	return (counter);
}
