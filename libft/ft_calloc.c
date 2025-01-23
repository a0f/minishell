/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:56:49 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:18:48 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;

	if (size != 0 && nmemb > __SIZE_MAX__ / size)
	{
		return (NULL);
	}
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
	{
		return (NULL);
	}
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
