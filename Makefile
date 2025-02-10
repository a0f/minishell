# Generated using 42 tools, manual changes may be overwritten

## Commands ##

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -g -I./
LDFLAGS = -L./libft -lft -lreadline
RM = rm -f

## Files ##

SRCS = src/main.c src/pool.c src/sb.c src/state.c src/tokenize.c src/pool_utils.c src/parse.c
OBJS = $(SRCS:src/%.c=obj/%.o)

## Configuration ##

NAME = minishell

## Compilation rules ##

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

## Cleaning rules ##

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	$(RM) $(OBJS) $(NAME)
	$(MAKE) -C libft fclean

re: fclean $(NAME)
	$(MAKE) -C libft re

## Libraries ##

libft/libft.a:
	$(MAKE) -C libft
