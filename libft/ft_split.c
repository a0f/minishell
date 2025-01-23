/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 18:58:26 by showard       #+#    #+#                 */
/*   Updated: 2024/10/15 14:33:10 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	substr_calc(const char *str, char deliminator)
{
	int	sub_strs;

	sub_strs = 0;
	while (*str != '\0' && *str == deliminator)
	{
		str++;
	}
	while (*str)
	{
		if (*str != deliminator)
		{
			sub_strs++;
			while (*str != deliminator && *str)
			{
				str++;
			}
		}
		else
		{
			str++;
		}
	}
	return (sub_strs);
}

static void	free_array(char **str)
{
	char	**temp;

	temp = str;
	while (*temp != NULL)
	{
		free(*temp);
		temp++;
	}
	free(str);
}

static char	**gen_array(char **str, char *src, int sub_strings, int delim)
{
	int	i;
	int	col;
	int	len;

	i = 0;
	col = 0;
	len = 0;
	while (col < sub_strings)
	{
		while (src[i] == delim && src[i])
			i++;
		len = 0;
		while ((src[i + len] != delim) && src[i + len])
			len++;
		str[col] = ft_substr(src, i, len);
		if (str[col] == NULL)
		{
			free_array(str);
			return (NULL);
		}
		col++;
		i += len;
	}
	str[col] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		sub_strs;

	sub_strs = substr_calc(s, c);
	str = (char **)malloc(((sub_strs + 1) * sizeof(char *)));
	if (str == NULL)
		return (NULL);
	return (gen_array(str, (char *)s, sub_strs, c));
}
