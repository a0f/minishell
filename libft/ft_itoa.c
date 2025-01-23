/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/12 16:01:31 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:19:33 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*str_rev(char *str)
{
	int		len;
	int		i;
	int		j;
	char	buff;

	len = ft_strlen(str) - 1;
	i = 0;
	j = len;
	while (i < j)
	{
		buff = str[i];
		str[i] = str[j];
		str[j] = buff;
		i++;
		j--;
	}
	return (str);
}

static int	calc_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
	}
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static char	*gen_str(char *str, int n)
{
	int	i;
	int	isneg;

	isneg = 0;
	i = 0;
	if (n < 0)
	{
		isneg = 1;
		n = n * -1;
	}
	while (n > 9)
	{
		str[i++] = (n % 10) + '0';
		n = n / 10;
	}
	str[i++] = n + '0';
	if (isneg == 1)
		str[i++] = '-';
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	str = malloc((calc_len(n) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str = gen_str(str, n);
	return (str_rev(str));
}
