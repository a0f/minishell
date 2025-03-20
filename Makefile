# Generated using 42 tools, manual changes may be overwritten

## Commands ##

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -I./ -g
LDFLAGS = -L./libft -lft -lreadline -ltermcap
RM = rm -f

## Files ##

SRCS = src/env.c src/debug_cmds.c src/map.c src/match_key.c src/built_ins.c src/state.c \
	   src/tokenize.c src/parse.c src/sb.c src/pool.c src/pool_utils.c src/main.c
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
