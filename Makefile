# Generated using 42 tools, manual changes may be overwritten

## Commands ##

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -I./ -g
LDFLAGS = -L./libft -lft
RM = rm -f

## Files ##

SRCS = src/env.c
OBJS = $(SRCS:src/%.c=obj/%.o)

## Configuration ##

NAME = minishell

## Compilation rules ##

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

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
