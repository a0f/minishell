/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:59:28 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:35:49 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		z;
	char	*str;

	i = 0;
	j = ft_strlen(s1) - 1;
	z = 0;
	while (s1[i] && (ft_strchr(set, s1[i]) != 0))
	{
		i++;
	}
	while (j > i && (ft_strchr(set, s1[j]) != 0))
	{
		j--;
	}
	str = (char *)malloc((j - i + 2) * sizeof(char));
	if (str == NULL)
	{
		return (NULL);
	}
	while (i <= j)
		str[z++] = s1[i++];
	str[z] = '\0';
	return (str);
}
