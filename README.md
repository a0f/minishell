# minishell

A small bash-like Unix shell in C, built at Codam (42 network) with [Matthias Wijnsma](https://github.com/Matthias1590).

## What it does

- Runs commands from `PATH` or by relative and absolute path
- Pipelines: `ls -l | grep .c | wc -l`
- Redirections: `<`, `>`, `>>` and heredocs (`<<`)
- Single and double quotes, `$VAR` expansion and `$?` for the last exit code
- Built-ins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Ctrl-C, Ctrl-D and Ctrl-\ behave as they do in bash

## Build and run

Needs a C compiler, `make` and the GNU readline library (`libreadline-dev` on Debian and Ubuntu).

```sh
make
./minishell
```

## How it works

A line goes through three stages:

1. **Tokenizer:** splits the input into words and operators, tracking quotes.
2. **Parser:** turns the tokens into commands with their arguments and redirections.
3. **Executor:** expands variables, sets up pipes and redirections, then forks and runs each command, or runs a built-in in the shell itself.

Memory is managed with pools grouped by lifetime (one per input line, one for the whole program), so a parse error or Ctrl-C frees everything for that line in one step.

## Who did what

- **Me:** the executor: running commands and pipelines, redirections and heredocs, the built-ins, the environment and signal handling.
- **Matthias:** the tokenizer, the parser, the memory pools and the string builder.
