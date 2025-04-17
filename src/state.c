/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 15:41:34 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/04/17 18:33:29 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define READ_END 0
#define WRITE_END 1

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

	tokens = tokenize(state, line, false, true);
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
#include <dirent.h>

void close_fds(void)
{
    DIR *fd_directory;
    struct dirent *read_val;
    int fd_to_close;
    int opendir_fd;

    fd_directory = opendir("/proc/self/fd");
	// set proper error
    if (fd_directory == NULL)
		printf("error lol\n");
    opendir_fd = dirfd(fd_directory);
    read_val = readdir(fd_directory);
    while (read_val != NULL)
    {
        fd_to_close = ft_atoi(read_val->d_name);
        if (fd_to_close > 2 && fd_to_close != opendir_fd)
        {
			// set proper error
            if (close(fd_to_close) == -1)
                printf("error lol\n");
        }
        read_val = readdir(fd_directory);
    }
    closedir(fd_directory);
}

pid_t	state_execve(t_state *state, char *cmd, char **args, char **envp)
{
	pid_t	pid;
	t_map	*path_node;
	char	**paths;

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE); // add real error
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		path_node = map_find(state->env, match_key_str, "PATH");
		paths = ft_split(path_node->value, ':');
		if (paths == NULL)
			exit(EXIT_FAILURE); // add real error
		cmd = find_valid_path(paths, cmd);
		if (cmd == NULL)
			exit(EXIT_FAILURE); // add real error
		close_fds();
		if (execve(cmd, args, envp) == -1)
			perror("execve error");
	}
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

char	**convert_env(t_map *state_env)
{
	t_map	*current;
	int		lst_size;
	char	**env;
	int		i;

	i = 0;
	lst_size = ft_mapsize(state_env);
	env = ft_calloc(lst_size, (sizeof(char *)));
	if (env == NULL)
		return (NULL); // error later
	current = state_env;
	while (current->next != NULL)
	{
		if (current->value != NULL)
			env[i] = ft_strjoin_path(current->key, current->value);
		else
			env[i] = ft_strdup(current->key);
		if (env[i] == NULL)
			return (NULL); // error later
		i++;
		current = current->next;
	}
	env[i] = NULL;
	i = 0;
	return (env);
}

int	find_builtin(t_state *state, t_cmd *cmd)
{
	state->last_exit_code = 0;
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

void	create_cmd_pipes(t_state *state, t_cmd *cmd)
{
	t_cmd	*temp_cmd;

	temp_cmd = cmd;
	while (temp_cmd)
	{
		if (temp_cmd == cmd)
		{
			temp_cmd = temp_cmd->pipe_into;
			continue ;
		}
		if (pipe(temp_cmd->pipe) == -1)
		{
			state_free(state);
			exit(1);
		}
		temp_cmd = temp_cmd->pipe_into;
	}
}

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void	input_heredoc(t_state *state, char *delimeter, int fd)
{
    int		lim_len;
    char	*gnl_r;
    int		len;
	t_tokens	*tokens;
	// t_cmd		*cmd;

    lim_len = ft_strlen(delimeter);
    while (1)
    {
        write(1, "> ", 2);
        gnl_r = get_next_line(0);
        if (gnl_r == NULL || (ft_strncmp(delimeter, gnl_r, lim_len) == 0
                && ft_strlen(delimeter) == ft_strlen(gnl_r) - 1))
        {
            free(gnl_r);
            break;
        }
		if (delimeter[0] == '\"' || delimeter[0] == '\'')
			tokens = tokenize(state, gnl_r, true, false);
		else 
			tokens = tokenize(state, gnl_r, true, true);
		if (!tokens)
		{
			return ;
		}
		while (tokens)
		{
			len = ft_strlen(tokens->value);
			if (write(fd, tokens->value, len) == -1)
			{
				close(fd);
				perror("write");
				exit(1);
			}
			tokens = tokens->next;
		}
        free(gnl_r);
    }
    // put file pointer back at start to read
    lseek(fd, 0, SEEK_SET);
}

bool	process_infile(t_state *state, t_cmd *cmd)
{
    t_input_file	*temp_in_file;
	int 		fd;
	
    temp_in_file = cmd->in_files;
    while (temp_in_file)
    {
        if (temp_in_file->type == INPUT_HEREDOC)
        {
			fd = open("/tmp/tmp_heredoc", O_CREAT | O_RDWR | O_TRUNC, 0744);
            if (fd == -1)
            {
                perror("open infile");
				return false;
            }
			input_heredoc(state, temp_in_file->value.delimeter, fd);
			// implement this check that bash does
			// bash: warning: here-document at line 70 delimited by end-of-file (wanted `delimiter')
        }
        else
        {
            fd = open(temp_in_file->value.path, O_RDONLY);
            if (fd == -1)
            {
                perror("open infile");
                return (false);
            }
        }
        close(cmd->fds[READ_END]);
        cmd->fds[READ_END] = fd;
        temp_in_file = temp_in_file->next;
    }
	return (true);
}

bool	process_outfile(t_cmd *cmd)
{
	t_output_file	*temp_out_file;
	int				fd;
	int				flags;

	temp_out_file = cmd->out_files;
	while (temp_out_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (temp_out_file->type == OUTPUT_TRUNCATE)
			flags |= O_TRUNC;
		else
			flags |= O_APPEND;
		fd = open(temp_out_file->path, flags, 0777); // todo: error check
		if (fd == -1)
		{
			perror("open outfile");
			return (false);
		}
		close(cmd->fds[WRITE_END]);
		cmd->fds[WRITE_END] = fd;
		temp_out_file = temp_out_file->next;
	}
	return (true);
}

void	get_previous_input(t_cmd *prev_cmd, t_cmd *cmd)
{
	if (prev_cmd->out_files == NULL)
		cmd->fds[READ_END] = cmd->pipe[READ_END];
	else
		cmd->fds[READ_END] = dup(STDIN_FILENO);
}

void	put_next_output(t_cmd *cmd)
{
	if (cmd->pipe_into->in_files == NULL)
		cmd->fds[WRITE_END] = cmd->pipe_into->pipe[WRITE_END];
	else
		cmd->fds[WRITE_END] = dup(STDOUT_FILENO);
}

bool	set_pipes(t_state *state, t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	t_cmd	*prev_cmd;

	temp_cmd = cmd;
	prev_cmd = NULL;
	while (temp_cmd)
	{
		if (temp_cmd == cmd)
			temp_cmd->fds[READ_END] = dup(STDIN_FILENO);
		// if not first command, get input from previous command
		else
			get_previous_input(prev_cmd, temp_cmd);
		// write output to stdout if last command
		if (!temp_cmd->pipe_into)
			temp_cmd->fds[WRITE_END] = dup(STDOUT_FILENO);
		// if not last command, write output to next command
		else
			put_next_output(temp_cmd);
		if (!process_infile(state, temp_cmd))
			return (false);
		if (!process_outfile(temp_cmd))
			return (false);
		prev_cmd = temp_cmd;
		temp_cmd = temp_cmd->pipe_into;
	}
	return (true);
}

void	restore_stds(int *original_stdin, int *original_stdout)
{
	dup2(*original_stdout, STDOUT_FILENO);
	close(*original_stdout);
	dup2(*original_stdin, STDIN_FILENO);
	close(*original_stdin);
}

void	link_cmd(t_cmd *cmd)
{
	if (dup2(cmd->fds[READ_END], STDIN_FILENO) == -1)
	{
		fprintf(stderr, "dup2 error\n");
		exit(1);
	}
	close(cmd->fds[READ_END]);
	if (dup2(cmd->fds[WRITE_END], STDOUT_FILENO) == -1)
	{
		fprintf(stderr, "dup2 error\n");
		exit(1);
	}
	close(cmd->fds[WRITE_END]);
}

void	state_run_cmd(t_state *state, t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	char	**envp;
	int		original_stdin;
	int		original_stdout;
	int		non_builtin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	create_cmd_pipes(state, cmd);
	if (!set_pipes(state, cmd))
	{
		close_fds();
		state->last_exit_code = 1;
		return ;
	}
	signal(SIGINT, SIG_IGN);
	temp_cmd = cmd;
	while (temp_cmd)
	{
		non_builtin = 0;
		link_cmd(temp_cmd);
		envp = convert_env(state->env);
		if (!find_builtin(state, temp_cmd))
		{
			non_builtin = 1;
			temp_cmd->pid = state_execve(state, temp_cmd->program,
					temp_cmd->args, envp);
			waitpid(temp_cmd->pid, NULL, 0);
		}
		if (!temp_cmd->pipe_into)
			break ;
		temp_cmd = temp_cmd->pipe_into;
	}
	if (non_builtin == 1)
		state->last_exit_code = get_exit_status(temp_cmd->pid);
	signal(SIGINT, sigint_interactive);
	restore_stds(&original_stdin, &original_stdout);
}
