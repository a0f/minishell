/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:58:53 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:01:10 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstlen;
	size_t	srclen;

	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	while (*dst != '\0')
		dst++;
	if (size <= dstlen)
	{
		return (size + srclen);
	}
	while ((size - 1) - dstlen > 0 && *src != '\0')
	{
		*dst = *src;
		size--;
		dst++;
		src++;
	}
	*dst = '\0';
	return (dstlen + srclen);
}
