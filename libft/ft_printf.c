/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/21 15:18:30 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 17:28:38 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdarg.h>
#include <unistd.h>

static int	determine_type(va_list args, char specifier, ssize_t *fcheck)
{
	int	count;

	count = 0;
	if (specifier == '%')
		count += ft_printf_perc(fcheck);
	else if (specifier == 'i' || specifier == 'd')
		count += ft_printf_int(va_arg(args, int), fcheck);
	else if (specifier == 'u')
		count += ft_printf_uint(va_arg(args, unsigned int), fcheck);
	else if (specifier == 'c')
		count += ft_printf_char(va_arg(args, int), fcheck);
	else if (specifier == 's')
		count += ft_printf_str(va_arg(args, const char *), fcheck);
	else if (specifier == 'p')
		count += ft_printf_ptr(va_arg(args, uintptr_t), fcheck);
	else if (specifier == 'x')
		count += ft_printf_lowerhex(va_arg(args, unsigned int), fcheck);
	else if (specifier == 'X')
		count += ft_printf_upperhex(va_arg(args, unsigned int), fcheck);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	int		count;
	ssize_t	fcheck;

	va_start(args, format);
	i = 0;
	count = 0;
	fcheck = 0;
	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			ft_write(1, &format[i], 1, &fcheck);
			count++;
		}
		else if (format[i] == '%' && format[i + 1] != '\0')
			count += determine_type(args, format[++i], &fcheck);
		i++;
	}
	va_end(args);
	if (fcheck == -1)
		return (-1);
	return (count);
}
