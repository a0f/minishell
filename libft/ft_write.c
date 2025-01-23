/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_write.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/24 15:39:40 by showard       #+#    #+#                 */
/*   Updated: 2024/10/24 17:25:59 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_write(int fd, const void *buffer, size_t count, ssize_t *fcheck)
{
	if (write(fd, buffer, count) == -1)
	{
		*fcheck = -1;
	}
}
