/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mwijnsma <mwijnsma@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/10 15:41:34 by mwijnsma      #+#    #+#                 */
/*   Updated: 2025/03/27 16:31:54 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	t_map	*path_node;
	char	**paths;

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
		env[i] = ft_strjoin_path(current->key, current->value);
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

void	input_heredoc(char *delimeter, int fd)
{
    int		lim_len;
    char	*gnl_r;
    int		len;

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
        len = ft_strlen(gnl_r);
        if (write(fd, gnl_r, len) == -1)
        {
            close(fd);
            free(gnl_r);
            perror("write");
            exit(1);
        }
        free(gnl_r);
    }
    // put file pointer back at start to read
    lseek(fd, 0, SEEK_SET);
}

void	process_infile(t_cmd *cmd)
{
    t_input_file	*temp_in_file;
	int 		fd;
	
    temp_in_file = cmd->in_files;
    while (temp_in_file)
    {
        if (temp_in_file->type == INPUT_HEREDOC)
        {
			fd = open(".tmp_heredoc", O_CREAT | O_RDWR | O_TRUNC, 0744);
            if (fd == -1)
            {
                perror("open infile");
                exit(1);
            }
			input_heredoc(temp_in_file->value.delimeter, fd);
			// implement this check that bash does
			// bash: warning: here-document at line 70 delimited by end-of-file (wanted `delimiter')
        }
        else
        {
            fd = open(temp_in_file->value.path, O_RDONLY);
            if (fd == -1)
            {
                perror("open infile");
                exit(1);
            }
        }
        close(cmd->fds[READ_END]);
        cmd->fds[READ_END] = fd;
        temp_in_file = temp_in_file->next;
    }
}

void	process_outfile(t_cmd *cmd)
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
		close(cmd->fds[WRITE_END]);
		cmd->fds[WRITE_END] = fd;
		temp_out_file = temp_out_file->next;
	}
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

void	set_pipes(t_cmd *cmd)
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
		process_infile(temp_cmd);
		process_outfile(temp_cmd);
		prev_cmd = temp_cmd;
		temp_cmd = temp_cmd->pipe_into;
	}
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

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	create_cmd_pipes(state, cmd);
	set_pipes(cmd);
	signal(SIGINT, SIG_IGN);
	temp_cmd = cmd;
	cmd_dump(cmd);
	while (temp_cmd)
	{
		link_cmd(temp_cmd);
		envp = convert_env(state->env);
		if (!find_builtin(state, temp_cmd))
			temp_cmd->pid = state_execve(state, temp_cmd->program,
					temp_cmd->args, envp);
		if (!temp_cmd->pipe_into)
			break ;
		temp_cmd = temp_cmd->pipe_into;
	}
	state->last_exit_code = get_exit_status(temp_cmd->pid);
	signal(SIGINT, sigint_interactive);
	restore_stds(&original_stdin, &original_stdout);
}
