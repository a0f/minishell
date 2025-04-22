/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:03:06 by showard       #+#    #+#                 */
/*   Updated: 2025/04/22 17:19:02 by showard       ########   odam.nl         */
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

int	get_exit_status(pid_t pid)
{
	int	status;

	wait4(pid, &status, 0, NULL);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (WEXITSTATUS(status));
}

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
	int				fd_to_close;
	int				opendir_fd;

	fd_directory = opendir("/proc/self/fd");
	if (fd_directory == NULL)
		return ;
	opendir_fd = dirfd(fd_directory);
	read_val = readdir(fd_directory);
	while (read_val != NULL)
	{
		fd_to_close = ft_atoi(read_val->d_name);
		if (fd_to_close > 2 && fd_to_close != opendir_fd && fd_to_close < 1024)
		{
			if (close(fd_to_close) == -1)
				return ;
		}
		read_val = readdir(fd_directory);
	}
	closedir(fd_directory);
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
