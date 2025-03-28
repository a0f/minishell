/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: showard <showard@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/21 12:36:04 by showard       #+#    #+#                 */
/*   Updated: 2025/03/27 15:22:40 by showard       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>

void parser(t_list *head, char *str)
{
	char *temp;
	t_list *temp_node;
	int len;
	int start;
	int i;

	start = 0;
	len = 0;
	i = 0;
	head = NULL;
	while(str[i] != '\0')
	{
		len++;
		if (str[i] == ' ')
		{
			temp = ft_strdup(" ");
			if (head == NULL)
				head = ft_lstnew(temp);
			else 
			{
				temp_node = ft_lstnew(temp);
				ft_lstadd_back(&head, temp_node);
			}
			start += len;
			len = 0;
		}
		else if (str[i + 1] == ' ')
		{
			temp = ft_substr(str, start, len);
			if (head == NULL)
				head = ft_lstnew(temp);
			else 
			{
				temp_node = ft_lstnew(temp);
				ft_lstadd_back(&head, temp_node);
			}
			start += len;
			len = 0;
		}
		i++;
	}
	temp = ft_substr(str, start, len);
	temp_node = ft_lstnew(temp);
	ft_lstadd_back(&head, temp_node);
	temp_node = head;
	while (temp_node != NULL)
	{
		printf("'%s'\n", (char *)temp_node->content);
		temp_node = temp_node->next;
	}
}

// Redirect("Makefile", Command("cat", "-e"))

/*

CMD = word [word...] [OPERATOR CMD]
OPERATOR = < > |

Command("cat", ["hello.txt"], Pipe(Command(...)))

cat hello.txt | rev
CMD = "cat" "hello.txt" | "rev"


< hello.txt cat | rev | rev | rev > out.txt

Pipe("cat < hello.txt", "cat")

"cat < hello.txt"
Redirect("hello.txt", "cat")


# Valid commands:
cat < hello.txt
< hello.txt cat
cat hello.txt | rev > out.txt
< hello.txt cat | rev > out.txt
< hello.txt cat | rev | rev > normal.txt



CMD = word [word...]
	= CMD < word
	= CMD > word





< set stdin
> set stdout

< hello.txt cat > h1.txt > h2.txt < h3.txt -e
// stdin = "hello.txt"
cmd = "cat"
// stdout = "h1.txt"
stdout = "h2.txt"
stdin = "h3.txt"
cmd += "-e"

parsesort | uniq > /tmp/error_report.txt

grep -i 'error' < /var/log/syslog
ReadFile(Word("/var/log/syslog"), Command("grep -i 'error'"))

awk '{print $1, $2, $3, $5}'
Command("awk '{print $1, $2, $3, $5}'")

sort
Command("sort")

uniq > /tmp/error_report.txt
WriteFile(Command("uniq"), Word("/tmp/error_report.txt"))


cat
Command("cat")

cat < hello.txt
Redirect(Word("hello.txt"), Command("cat"))

< hello.txt cat
Redirect(Word("hello.txt"), Command("cat"))

< hello.txt
Redirect(Word("hello.txt"), NULL)

*/

t_command *command_from_string(char *string) {
	
}

void state_execute(t_state *state, t_command *command) {
	
}

int	main(void)
{
	t_list *head;
	char *line;

	head = NULL;
	while (1)
	{
		line = readline("test> ");
		if (line == NULL)
			break;
		parser(head, line);
		add_history(line);
		free(line);
	}
	free(line);
	rl_clear_history();

	return 0;
}

// first make tokenizer
// then make parser
// then make excecuter


// Tokenizer example
// input = < input.txt cat -e > output.txt
// tokens = [<][ ][input.txt][ ][cat][ ][-e][ ][>][ ][output.txt]



/*

- environment vars (take from envp at startup, modify in memory)
// - tokenize, parse, and interpret input

things needed for interpeting:
  - keep track of child

*/

typedef struct s_env_entry {
	char *name;
	char *value;
	struct s_env_entry *next;
} t_env_entry;

typedef struct {
	t_env_entry *first;
} t_env;

t_env *env_from_envp(char **envp) {

}

void env_free(t_env *env) {
	
}

char *env_get(t_env *env, char *name) {
	t_env_entry *entry;

	entry = env->first;
	while (entry != NULL) {
		if (strcmp(entry->name, name) == 0) {
			return entry->value;
		}

		entry = entry->next;
	}
}

void env_set(t_env *env, char *name, char *value);

typedef struct {
	t_env *env;
	pid_t running_command;
	int last_exit_code;
} t_state;





void builtin_echo(int argc, char **argv) {
	
}

void builtin_env(t_state *state) {
	t_env_entry *entry = state->env->first;
	
	while (entry != NULL) {
		
	}
}

typedef struct {
	ms_env *env;
} ms;

typedef struct {
	ms_env_pair *pairs;
} ms_env;
