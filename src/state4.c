/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state4.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/22 17:07:40 by showard       #+#    #+#                 */
/*   Updated: 2025/04/22 17:20:47 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
		return (NULL);
	current = state_env;
	while (current->next != NULL)
	{
		if (current->value != NULL)
			env[i] = ft_strjoin_path(current->key, current->value);
		else
			env[i] = ft_strdup(current->key);
		if (env[i] == NULL)
			return (free_2d(env), NULL);
		i++;
		current = current->next;
	}
	env[i] = NULL;
	return (env);
}

bool	is_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->program, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "export") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "env") == 0)
		return (true);
	if (ft_strcmp(cmd->program, "exit") == 0)
		return (true);
	return (false);
}

int	find_builtin(t_state *state, t_cmd *cmd)
{
	state->last_exit_code = 0;
	if (ft_strcmp(cmd->program, "echo") == 0)
		echo(state, &cmd->args[1]);
	else if (ft_strcmp(cmd->program, "cd") == 0)
		cd(state, &cmd->args[1]);
	else if (ft_strcmp(cmd->program, "pwd") == 0)
		pwd(state);
	else if (ft_strcmp(cmd->program, "export") == 0)
		export(&cmd->args[1], state);
	else if (ft_strcmp(cmd->program, "unset") == 0)
		unset(state, cmd->args[1]);
	else if (ft_strcmp(cmd->program, "env") == 0)
		env(state);
	else if (ft_strcmp(cmd->program, "exit") == 0)
		exit_ms(state, cmd->args);
	else
		return (false);
	return (true);
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

int	heredoc_getline(char **gnl_r, char *delimeter, int lim_len)
{
	*gnl_r = get_next_line(0);
	if (*gnl_r == NULL || (ft_strncmp(delimeter, *gnl_r, lim_len) == 0
			&& ft_strlen(delimeter) == ft_strlen(*gnl_r) - 1))
	{
		free(*gnl_r);
		if (g_signal == 2)
			return (-1);
		return (1);
	}
	return (0);
}
