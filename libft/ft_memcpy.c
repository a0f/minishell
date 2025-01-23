/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcpy.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:57:31 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 13:59:58 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dptr;
	unsigned char	*sptr;

	if (dest == NULL && src == NULL)
	{
		if (n != 0)
		{
			return (NULL);
		}
	}
	i = 0;
	dptr = (unsigned char *) dest;
	sptr = (unsigned char *) src;
	while (i < n)
	{
		dptr[i] = sptr[i];
		i++;
	}
	return (dest);
}
