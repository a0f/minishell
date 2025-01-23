/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:57:36 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:00:02 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*b_dest;
	unsigned char	*b_src;

	b_dest = dest;
	b_src = (unsigned char *) src;
	if (dest < src)
	{
		return (ft_memcpy(dest, src, n));
	}
	else if (dest > src)
	{
		while (n != 0)
		{
			n--;
			b_dest[n] = b_src[n];
		}
	}
	return (dest);
}
