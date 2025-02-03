/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_str.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/22 16:50:14 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 16:02:04 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>

int	ft_printf_str(const char *str, ssize_t *fcheck)
{
	if (str == NULL)
	{
		ft_write(1, "(null)", 6, fcheck);
		return (6);
	}
	ft_putstr_fd((char *)str, 1, fcheck);
	return (ft_strlen(str));
}
