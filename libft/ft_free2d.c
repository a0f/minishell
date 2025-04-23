/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free2d.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/09 18:52:06 by showard       #+#    #+#                 */
/*   Updated: 2025/04/23 16:03:38 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	**free_2d(char **str)
{
	char	**temp;

	if (str == NULL)
		return (NULL);
	temp = str;
	while (*temp != NULL)
	{
		free(*temp);
		temp++;
	}
	free(str);
	return (NULL);
}
