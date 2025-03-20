/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwijnsma <mwijnsma@codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:41:34 by mwijnsma          #+#    #+#             */
/*   Updated: 2025/03/20 15:35:03 by mwijnsma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

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
	pid_t	pid;
	t_map 	*path_node;
	char 	**paths;

	pid = fork();
	if (pid == -1)
		printf("error lol\n");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		path_node = map_find(state->env, match_key_str, "PATH");
		paths = ft_split(path_node->value, ':');
		cmd = find_valid_path(paths, cmd);
		execve(cmd, args, envp);
	}
	else
	{
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

#define READ_END 0
#define WRITE_END 1

// void	exec_builtin(t_state *state, t_cmd *cmd)
// {
	
// }
void	state_run_cmd(t_state *state, t_cmd *cmd)
{
	t_cmd			*temp_cmd;
	t_cmd			*prev_cmd;
	t_input_file	*temp_in_file;
	t_output_file	*temp_out_file;
	int				fd;
	int				original_stdout;
	int				original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);

	// dup2(cmd->fds[1], STDOUT_FILENO);
	// dup2(cmd->fds[0], STDIN_FILENO);

	// // echo 123 > out.txt
	// printf("123\n");  // goes to fds[1] (the output file)

	// // restore stdout
	// dup2(actual_stdout, STDOUT_FILENO);

	// printf("> ");  // prompt goes to actual stdout

	// create pipes
	temp_cmd = cmd;
	while (temp_cmd)
	{
		if (temp_cmd == cmd)
		{
			temp_cmd = temp_cmd->pipe_into;
			continue;
		}

		if (pipe(temp_cmd->pipe) == -1)
		{
			state_free(state);
			exit(1);
		}
		printf("+ allocated pipe { read %d, write %d }\n", temp_cmd->pipe[READ_END], temp_cmd->pipe[WRITE_END]);
		// printf("+ '%s' input pipe = %d, output pipe = %d\n", temp_cmd->program, temp_cmd->pipe[0], temp_cmd->pipe[1]);
		temp_cmd = temp_cmd->pipe_into;
	}

	// 5 = read end of pipe
	// 6 = write end of pipe
	// 7 = stdin
	// 8 = stdout

	// link fds
	temp_cmd = cmd;
	prev_cmd = NULL;
	while (temp_cmd)
	{
		if (temp_cmd == cmd)
		{
			temp_cmd->fds[READ_END] = dup(STDIN_FILENO);
			printf("+ duplicated stdin, %d points to stdin now\n", temp_cmd->fds[READ_END]);
		}
		// if not first command, get input from previous command
		else
		{
			if (prev_cmd->out_files == NULL)
				temp_cmd->fds[READ_END] = temp_cmd->pipe[READ_END];
			else
			{
				temp_cmd->fds[READ_END] = dup(STDIN_FILENO);
				printf("+ duplicated stdin, %d points to stdin now\n", temp_cmd->fds[READ_END]);
			}
		}
		// write output to stdout if last command
		if (!temp_cmd->pipe_into)
		{
			temp_cmd->fds[WRITE_END] = dup(STDOUT_FILENO);
			printf("+ duplicated stdout, %d points to stdout now\n", temp_cmd->fds[WRITE_END]);
		}
		// if not last command, write output to next command
		else
		{
			if (temp_cmd->pipe_into->in_files == NULL)
			{
				temp_cmd->fds[WRITE_END] = temp_cmd->pipe_into->pipe[WRITE_END];
			}
			else
			{
				temp_cmd->fds[WRITE_END] = dup(STDOUT_FILENO);
				printf("+ duplicated stdout, %d points to stdout now\n", temp_cmd->fds[WRITE_END]);
			}
		}
		temp_in_file = temp_cmd->in_files;
		while (temp_in_file)
		{
		 	fd = open(temp_in_file->value.path, O_RDONLY);  // todo: error check and handle heredocs
			close(temp_cmd->fds[READ_END]);
			temp_cmd->fds[READ_END] = fd;
			temp_in_file = temp_in_file->next;
		}
		temp_out_file = temp_cmd->out_files;
		while (temp_out_file)
		{
			int flags = O_WRONLY | O_CREAT;
			if (temp_out_file->type == OUTPUT_TRUNCATE) {
				flags |= O_TRUNC;
			} else {
				flags |= O_APPEND;
			}
			fd = open(temp_out_file->path, flags, 0777);  // todo: error check
			close(temp_cmd->fds[WRITE_END]);
			temp_cmd->fds[WRITE_END] = fd;
			temp_out_file = temp_out_file->next;
		}
		prev_cmd = temp_cmd;
		temp_cmd = temp_cmd->pipe_into;
	}

	temp_cmd = cmd;
	while (temp_cmd)
	{
		fprintf(stderr, "+ program %s\n", temp_cmd->program);
		fprintf(stderr, "+   reads from %d\n", temp_cmd->fds[READ_END]);
		fprintf(stderr, "+   writes to %d\n", temp_cmd->fds[WRITE_END]);

		temp_cmd = temp_cmd->pipe_into;
	}

	signal(SIGINT, SIG_IGN);

	temp_cmd = cmd;
	while (temp_cmd)
	{
		fprintf(stderr, "make %d point to file of %d\n", STDIN_FILENO, temp_cmd->fds[READ_END]);
		if (dup2(temp_cmd->fds[READ_END], STDIN_FILENO) == -1) {
			fprintf(stderr, "dup2 error\n");
			exit(1);
		}
		close(temp_cmd->fds[READ_END]);
		fprintf(stderr, "make %d point to file of %d\n", STDOUT_FILENO, temp_cmd->fds[WRITE_END]);
		if (dup2(temp_cmd->fds[WRITE_END], STDOUT_FILENO) == -1) {
			fprintf(stderr, "dup2 error\n");
			exit(1);
		}
		close(temp_cmd->fds[WRITE_END]);

		// // execute temp_cmd
		// if (!find_builtin(state, temp_cmd))
		// {
		char 	**envp;

		// cmd_dump(cmd);
		fprintf(stderr, "+ running %s\n", temp_cmd->program);

		envp = convert_env(state->env);
		if (!find_builtin(state, temp_cmd))
		{
			temp_cmd->pid = state_execve(state, temp_cmd->program, temp_cmd->args, envp);
		}

		// pid = state_execve(state, cmd->program, cmd->args, envp);
		// // run second command
	// 	// // run third command
	// 	// // pid = run_lsat_Command();  // using state_execve
		// state->last_exit_code = get_exit_status(pid);
		// }

		if (!temp_cmd->pipe_into) {
			break;
		}
		temp_cmd = temp_cmd->pipe_into;
	}
	state->last_exit_code = get_exit_status(temp_cmd->pid);

	signal(SIGINT, sigint_interactive);

	// restore stdout and stdin
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
