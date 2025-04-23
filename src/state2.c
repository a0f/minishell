/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: showard <showard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:06 by showard           #+#    #+#             */
/*   Updated: 2025/04/23 10:18:42 by showard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*path_complete(char const *s1, char const *s2)
{
	int		total_len;
	char	*str;
	int		i;

	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
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
	str[i] = '/';
	i++;
	while (*s2)
	{
		str[i] = *s2;
		i++;
		s2++;
	}
	str[i] = '\0';
	return (str);
}

char	*find_valid_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i] != NULL)
	{
		temp = path_complete(paths[i], cmd);
		if (temp == NULL)
			return (NULL);
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		i++;
	}
	return (ft_strdup(cmd));
}

void	close_fds(void)
{
	DIR				*fd_directory;
	struct dirent	*read_val;
	int				fds_to_close[1024];
	int				count;
	int				current_fd;

	count = 0;
	fd_directory = opendir("/proc/self/fd");
	if (fd_directory == NULL)
		return ;
	while (1)
	{
		read_val = readdir(fd_directory);
		if (read_val == NULL || count >= 1024)
			break ;
		current_fd = ft_atoi(read_val->d_name);
		if (current_fd > 2)
			fds_to_close[count++] = current_fd;
	}
	closedir(fd_directory);
	while (count > 0)
	{
		close(fds_to_close[count - 1]);
		count--;
	}
}

void	check_cmd(t_state *state, char *cmd)
{
	struct stat	buffer;

	if (!ft_strchr(cmd, '/'))
	{
		(write_stderr("minishell: "), write_stderr(cmd));
		write_stderr(": command not found\n");
		(state_free(state), exit(127));
	}
	if (stat(cmd, &buffer) == -1 && errno == ENOENT)
	{
		(write_stderr("minishell: "), write_stderr(cmd));
		write_stderr(": No such file or directory\n");
		(state_free(state), exit(127));
	}
	if (S_ISDIR(buffer.st_mode))
	{
		(write_stderr("minishell: "), write_stderr(cmd));
		(write_stderr(": Is a directory\n"), state_exit(state, 126));
	}
	if (access(cmd, X_OK) == -1)
	{
		(write_stderr("minishell: "), write_stderr(cmd));
		write_stderr(": Permission denied\n");
		(state_free(state), exit(126));
	}
}
