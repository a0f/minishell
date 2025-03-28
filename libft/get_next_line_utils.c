/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/25 15:08:43 by showard       #+#    #+#                 */
/*   Updated: 2024/11/29 19:19:02 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen_gnl(const char *s)
{
	size_t	len;

	len = 0;
	while (*s != '\0')
	{
		s++;
		len++;
	}
	return (len);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (*s == (char)c)
	{
		return ((char *)s);
	}
	return (0);
}

char	*ft_strdup_gnl(const char *s)
{
	int		length;
	int		i;
	char	*str;

	length = 0;
	i = 0;
	while (s[length])
		length++;
	str = (char *)malloc((length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	int		total_len;
	char	*str;
	int		i;

	total_len = ft_strlen_gnl(s1) + ft_strlen_gnl(s2);
	str = (char *)malloc((total_len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*s1)
	{
		str[i] = *s1;
		i++;
		s1++;
	}
	while (*s2)
	{
		str[i] = *s2;
		i++;
		s2++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	char			*str;
	unsigned int	length;

	length = ft_strlen_gnl(s);
	if (start > length)
		return (ft_strdup_gnl(""));
	if (len > length - start)
		len = length - start;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start] != '\0')
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}
