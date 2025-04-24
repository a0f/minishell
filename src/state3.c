/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state3.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:04:53 by showard       #+#    #+#                 */
/*   Updated: 2025/04/24 16:44:00 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	state_execve_path(t_state *state, char **cmd, char *args[],
		t_map *path_node)
{
	char	**paths;

	paths = ft_split(path_node->value, ':');
	if (paths == NULL)
		(state_free(state), exit(EXIT_FAILURE));
	*cmd = find_valid_path(state->static_pool, paths, *cmd);
	if (*cmd == NULL)
		(free_2d(paths), state_free(state), exit(EXIT_FAILURE));
	if (ft_strchr(*cmd, '/') == NULL)
	{
		*cmd = pool_strdup(state->static_pool, args[0]);
		if (*cmd == NULL)
			(free_2d(paths), state_free(state), exit(EXIT_FAILURE));
	}
	free_2d(paths);
}

void	state_execve_child(t_state *state, char *cmd, char *args[],
		char *envp[])
{
	t_map	*path_node;

	close_fds();
	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
	path_node = map_find(state->env, match_key_str, "PATH");
	if (path_node && cmd[0] != '.' && cmd[1] != '/')
		state_execve_path(state, &cmd, args, path_node);
	if (cmd == NULL)
		(state_free(state), exit(EXIT_FAILURE));
	check_cmd(state, (void **)&cmd);
	if (execve(cmd, args, envp) == -1)
	{
		perror("minishell");
		(state_free(state), exit(EXIT_FAILURE));
	}
}

pid_t	state_execve(t_state *state, char *cmd, char **args, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		(state_free(state), exit(EXIT_FAILURE));
	if (pid == 0)
		state_execve_child(state, cmd, args, envp);
	else
		state->running_command = pid;
	return (pid);
}

int	ft_mapsize(t_map *map)
{
	int	counter;

	counter = 0;
	while (map != NULL)
	{
		counter++;
		map = map->next;
	}
	return (counter);
}

char	*ft_strjoin_path(char const *s1, char const *s2)
{
	int		total_len;
	char	*str;
	int		i;

	total_len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc((total_len + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*s1)
	{
		str[i] = *s1;
		i++;
		s1++;
	}
	str[i] = '=';
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
