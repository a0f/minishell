/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:18:23 by showard           #+#    #+#             */
/*   Updated: 2025/04/22 15:24:12 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

char	*ft_strcpy_gnl(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

#include <stdio.h>

void	complete_line(char *buffer, char **full_line, int fd)
{
	int		amount_read;
	char	*temp;

	amount_read = 1;
	while (amount_read > 0 && ft_strchr_gnl(*full_line, '\n') == NULL)
	{
		amount_read = read(fd, buffer, BUFFER_SIZE);
		if (amount_read == -1)
		{
			free(*full_line);
			*full_line = NULL;
			return ;
		}
		if (amount_read == 0)
			break ;
		buffer[amount_read] = '\0';
		temp = *full_line;
		*full_line = ft_strjoin_gnl(temp, buffer);
		if (*full_line == NULL)
		{
			free(temp);
			return ;
		}
		free(temp);
	}
}

void	clean_lines(char *remainder, char **full_line, char **r_line)
{
	int		len;
	char	*nl_ptr;

	nl_ptr = ft_strchr_gnl(*full_line, '\n');
	if (nl_ptr != NULL)
	{
		len = nl_ptr - *full_line;
		*r_line = ft_substr_gnl(*full_line, 0, len + 1);
		if (*r_line == NULL)
		{
			free(*full_line);
			return ;
		}
		ft_strcpy_gnl(remainder, nl_ptr + 1);
	}
	else
	{
		*r_line = ft_strdup_gnl(*full_line);
		if (*r_line == NULL)
		{
			free(*full_line);
			return ;
		}
	}
	free(*full_line);
}

char	*get_next_line(int fd)
{
	static char	remainder[BUFFER_SIZE + 1];
	char		*r_line;
	char		*full_line;

	if (fd < 0)
		return (NULL);
	full_line = ft_strdup_gnl(remainder);
	if (full_line == NULL)
		return (NULL);
	complete_line(remainder, &full_line, fd);
	if (ft_strchr_gnl(remainder, '\n') == NULL)
		remainder[0] = '\0';
	if (full_line == NULL)
	{
		free(full_line);
		return (NULL);
	}
	clean_lines(remainder, &full_line, &r_line);
	if (r_line == NULL || (remainder[0] == '\0' && r_line[0] == '\0'))
	{
		free(r_line);
		return (NULL);
	}
	return (r_line);
}
