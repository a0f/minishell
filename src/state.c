/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 15:41:34 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/03/20 12:01:19 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	*state_new(void)
{
	t_state	*state;

	state = ft_calloc(1, sizeof(t_state));
	if (!state)
	{
		return (NULL);
	}
	state->program_pool = pool_new();
	if (!state->program_pool)
	{
		state_free(state);
		return (NULL);
	}
	state->parser_pool = pool_new();
	if (!state->parser_pool)
	{
		state_free(state);
		return (NULL);
	}
	// todo: init other state vars
	return (state);
}

void	state_free(t_state *state)
{
	// todo: free other state vars
	if (state->program_pool)
	{
		pool_free(state->program_pool);
	}
	if (state->parser_pool)
	{
		pool_free(state->parser_pool);
	}
	rl_clear_history();
	free(state);
}

void	state_exit(t_state *state, int code)
{
	state_free(state);
	exit(code);
}

void	state_run_string(t_state *state, char *line)
{
	t_tokens	*tokens;
	t_cmd		*cmd;

	tokens = tokenize(state->parser_pool, line);
	if (!tokens)
	{
		return ;
	}
	cmd = parse(state->parser_pool, tokens);
	if (!cmd)
	{
		return ;
	}
	state_run_cmd(state, cmd);
}

void	sigint_interactive(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

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
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		i++;
	}
	return (ft_strdup(cmd));
}

pid_t	state_execve(t_state *state, char *cmd, char **args, char **envp)
{
	int		fd[2];
	pid_t	pid;
	t_map 	*path_node;
	char 	**paths;

		
	if (pipe(fd) == -1)
		printf("error opening pipe\n");
	pid = fork();
	if (pid == -1)
		printf("error lol\n");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1
			|| close(fd[1]) == -1)
			printf("error lol\n");
		path_node = map_find(state->env, match_key_str, "PATH");
		paths = ft_split(path_node->value, ':');
		cmd = find_valid_path(paths, cmd);
		execve(cmd, args, envp);
	}
	else
	{
		if (dup2(STDOUT_FILENO, fd[0]) == -1 || close(fd[0]) == -1
			|| close(fd[1]) == -1)
		state->running_command = pid;
	}
	return (pid);
}

// pid_t	state_execve(t_state *state, char *cmd, char **args, char **envp)
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	t_map 	*path_node;
// 	char 	**paths;


// 	if (pipe(fd) == -1)
// 		printf("error opening pipe\n");
// 	pid = fork();
// 	if (pid == -1)
// 		printf("error lol\n");
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1
// 			|| close(fd[1]) == -1)
// 			printf("error lol\n");
// 		path_node = map_find(state->env, match_key_str, "PATH");
// 		paths = ft_split(path_node->value, ':');
// 		cmd = find_valid_path(paths, cmd);
// 		execve(cmd, args, envp);
// 	}
// 	else
// 	{
// 		if (dup2(STDOUT_FILENO, fd[0]) == -1 || close(fd[0]) == -1
// 			|| close(fd[1]) == -1)
// 		state->running_command = pid;
// 	}
// 	return (pid);
// }

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

char **convert_env(t_map *state_env)
{
	t_map *current;
	int	lst_size;
	char **env;
	int i;

	i = 0;
	lst_size = ft_mapsize(state_env);
	env = ft_calloc(lst_size, (sizeof(char *)));
	if (env == NULL)
		return (NULL); //error later
	current = state_env;
	while (current->next != NULL)
	{
		env[i] = ft_strjoin_path(current->key, current->value);
		if (env[i] == NULL)
			return (NULL); //error later
		i++;
		current = current->next;
	}
	env[i] = NULL;
	i = 0;
	return (env);
}


int	find_builtin(t_state *state, t_cmd *cmd)
{
	if (ft_strcmp(cmd->program, "echo") == 0)
		return (echo(state, &cmd->args[1]), true);
	else if (ft_strcmp(cmd->program, "cd") == 0)
		return (cd(state, &cmd->args[1]), true);
	else if (ft_strcmp(cmd->program, "pwd") == 0)
		return (pwd(state), true);
	else if (ft_strcmp(cmd->program, "export") == 0)
		return (export(&cmd->args[1], state), true);
	else if (ft_strcmp(cmd->program, "unset") == 0)
		return (unset(state, cmd->args[1]), true);
	else if (ft_strcmp(cmd->program, "env") == 0)
		return (env(state), true);
	else if (ft_strcmp(cmd->program, "exit") == 0)
		return (exit_ms(state, cmd->args), true);
	return (false);
}

// void	exec_builtin(t_state *state, t_cmd *cmd)
// {
	
// }
void	state_run_cmd(t_state *state, t_cmd *cmd)
{
	// pid_t	pid = 0;
	// // t_cmd 	*current;
	char 	**envp;

	cmd_dump(cmd);
	signal(SIGINT, SIG_IGN);
	envp = convert_env(state->env);
	// determine if it is a built_in and execute it
	find_builtin(state, cmd);
	
	// pid = state_execve(state, cmd->program, cmd->args, envp);
	// // envp = convert_env(state->env);
	// // pid = state_execve(state, cmd->program, cmd->args, envp);
	// // run second command
// 	// // run third command
// 	// // pid = run_lsat_Command();  // using state_execve
// 	state->last_exit_code = get_exit_status(pid);
// 	signal(SIGINT, sigint_interactive);
}
