# Generated using 42 tools, manual changes may be overwritten

## Commands ##

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -I./ -g
LDFLAGS = -L./libft -lft -lreadline -ltermcap
RM = rm -f

## Files ##

SRCS = src/built_ins2.c src/env2.c src/map.c src/parse2.c src/pool.c src/state.c src/tokenize.c \
	src/built_ins.c src/env.c src/match_key2.c src/parse3.c src/pool_utils.c src/tokenize2.c \
	src/debug_cmds.c src/main.c src/match_key.c src/parse.c src/sb.c src/tokenize3.c
OBJS = $(SRCS:src/%.c=obj/%.o)

## Configuration ##

NAME = minishell

## Compilation rules ##

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

## Cleaning rules ##

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	$(RM) $(OBJS) $(NAME)
	$(MAKE) -C libft fclean

re: fclean $(NAME)

## Libraries ##

libft/libft.a:
	$(MAKE) -C libft all
