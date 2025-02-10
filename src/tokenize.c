#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sb.h"
#include "pool.h"
#include "libft.h"

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_LT,
    TOKEN_GT,
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_tokens;

t_tokens *tokens_new(t_pool *pool, t_token_type type) {
    t_tokens *token = pool_calloc(pool, sizeof(t_tokens));
    if (!token) {
        return (NULL);
    }

    token->type = type;
    token->next = NULL;

    return (token);
}

t_tokens *tokens_last(t_tokens *tokens) {
    while (tokens && tokens->next) {
        tokens = tokens->next;
    }
    return tokens;
}

t_tokens *tokens_add(t_pool *pool, t_tokens **tokens, t_token_type type) {
    if (!*tokens) {
        *tokens = tokens_new(pool, type);
        return *tokens;
    } else {
        t_tokens *last = tokens_last(*tokens);
        last->next = tokens_new(pool, type);
        return last->next;
    }
}

char *tokenize_single_quotes(t_pool *pool, t_tokens **tokens, char *cmd) {
    t_tokens *token = tokens_add(pool, tokens, TOKEN_WORD);
    if (!token) {
        return (NULL);
    }
    t_sb *word = sb_new(pool);
    if (!word) {
        return (NULL);
    }
    cmd++;
    while (*cmd && *cmd != '\'') {
        sb_append_char(word, *cmd);
        cmd++;
    }
    if (!*cmd) {
        printf("Unmatched quote\n");
        return NULL;
    }
    cmd++;
    token->value = sb_build(word);
    return cmd;
}

char *tokenize_double_quotes(t_pool *pool, t_tokens **tokens, char *cmd) {
    t_tokens *token = tokens_add(pool, tokens, TOKEN_WORD);
    t_sb *word = sb_new(pool);
    cmd++;
    while (*cmd && *cmd != '"') {
        sb_append_char(word, *cmd);
        cmd++;
    }
    if (!*cmd) {
        printf("Unmatched quote\n");
        return NULL;
    }
    cmd++;
    token->value = sb_build(word);
    return cmd;
}

char *tokenize_pipe(t_pool *pool, t_tokens **tokens, char *cmd) {
    tokens_add(pool, tokens, TOKEN_PIPE);
    return cmd + 1;
}

char *tokenize_lt(t_pool *pool, t_tokens **tokens, char *cmd) {
    tokens_add(pool, tokens, TOKEN_LT);
    return cmd + 1;
}

char *tokenize_gt(t_pool *pool, t_tokens **tokens, char *cmd) {
    tokens_add(pool, tokens, TOKEN_GT);
    return cmd + 1;
}

char *tokenize_word(t_pool *pool, t_tokens **tokens, char *cmd) {
    t_tokens *token = tokens_add(pool, tokens, TOKEN_WORD);
    t_sb *word = sb_new(pool);
    while (*cmd && *cmd != ' ' && *cmd != '|' && *cmd != '\'' && *cmd != '"') {
        sb_append_char(word, *cmd);
        cmd++;
    }
    token->value = sb_build(word);
    return cmd;
}

char *tokenize_single(t_pool *pool, t_tokens **tokens, char *cmd) {
    char c = *cmd;
    if (c == '\'') {
        return tokenize_single_quotes(pool, tokens, cmd);
    } else if (c == '"') {
        return tokenize_double_quotes(pool, tokens, cmd);
    } else if (c == '|') {
        return tokenize_pipe(pool, tokens, cmd);
    } else if (c == '<') {
        return tokenize_lt(pool, tokens, cmd);
    } else if (c == '>') {
        return tokenize_gt(pool, tokens, cmd);
    } else {
        return tokenize_word(pool, tokens, cmd);
    }
}

char *preprocess(t_pool *pool, char *cmd) {
    bool in_single = false, in_double = false;
    t_sb *out = sb_new(pool);
    size_t i = 0;

    while (cmd[i]) {
        if (cmd[i] == '\'' && !in_double) {
            in_single = !in_single;
            sb_append_char(out, cmd[i]);
            i++;
            continue;
        }
        if (cmd[i] == '"' && !in_single) {
            in_double = !in_double;
            sb_append_char(out, cmd[i]);
            i++;
            continue;
        }
        if (cmd[i] == '$' && !in_single) {
            i++;
            t_sb *var_sb = sb_new(pool);
            while (cmd[i] && cmd[i] != '\'' && cmd[i] != '"' && cmd[i] != ' ') {
                sb_append_char(var_sb, cmd[i]);
                i++;
            }
            char *var_value = getenv(sb_build(var_sb));
            if (var_value) {
                sb_append(out, var_value);
            }
        } else {
            sb_append_char(out, cmd[i]);
            i++;
        }
    }

    if (in_single || in_double) {
        printf("Unmatched quote\n");
        return NULL;
    }

    return sb_build(out);
}

t_tokens *tokenize(t_pool *pool, char *cmd) {
    t_tokens *tokens = NULL;
    cmd = preprocess(pool, cmd);
    if (!cmd) {
        return NULL;
    }
    while (*cmd) {
        while (*cmd == ' ') {
            cmd++;
        }
        if (*cmd) {
            cmd = tokenize_single(pool, &tokens, cmd);
            if (!cmd) {
                return NULL;
            }
        }
    }
    return (tokens);
}

typedef struct s_cmd {
    char *program;
    char **args;
    char **in_files;
    char **out_files;
    struct s_cmd *pipe_into;
} t_cmd;

t_cmd *cmd_new(t_pool *pool) {
    t_cmd *cmd = pool_calloc(pool, sizeof(t_cmd));
    if (!cmd) {
        return (NULL);
    }
    cmd->args = pool_calloc(pool, sizeof(char *));
    if (!cmd->args) {
        return (NULL);
    }
    return cmd;
}

bool cmd_append_arg(t_pool *pool, t_cmd *cmd, char *arg) {
    size_t len = 0;
    while (cmd->args[len]) {
        len++;
    }

    cmd->args = pool_realloc(pool, cmd->args, (len + 2) * sizeof(char *));
    if (!cmd->args) {
        return (false);
    }

    cmd->args[len] = arg;
    cmd->args[len + 1] = NULL;

    return (true);
}

bool cmd_append_in_file(t_pool *pool, t_cmd *cmd, char *in_file) {
    size_t len = 0;
    while (cmd->in_files[len]) {
        len++;
    }

    cmd->in_files = pool_realloc(pool, cmd->in_files, (len + 2) * sizeof(char *));
    if (!cmd->in_files) {
        return (false);
    }

    cmd->in_files[len] = in_file;
    cmd->in_files[len + 1] = NULL;

    return (true);
}

bool cmd_append_out_file(t_pool *pool, t_cmd *cmd, char *out_file) {
    size_t len = 0;
    while (cmd->out_files[len]) {
        len++;
    }

    cmd->out_files = pool_realloc(pool, cmd->out_files, (len + 2) * sizeof(char *));
    if (!cmd->out_files) {
        return (false);
    }

    cmd->out_files[len] = out_file;
    cmd->out_files[len + 1] = NULL;

    return (true);
}

char *pool_strdup(t_pool *pool, char *str) {
    char *dup = pool_calloc(pool, ft_strlen(str) + 1);
    if (!dup) {
        return (NULL);
    }
    ft_memcpy(dup, str, ft_strlen(str));
    return (dup);
}

t_cmd *parse(t_pool *pool, t_tokens *tokens) {
    t_cmd *first = cmd_new(pool);

    t_cmd *cmd = first;
    while (tokens) {
        if (tokens->type == TOKEN_WORD) {
            if (!cmd->program) {
                cmd->program = pool_strdup(pool, tokens->value);
                if (!cmd->program) {
                    return (NULL);
                }
            } else {
                if (!cmd_append_arg(pool, cmd, pool_strdup(pool, tokens->value))) {
                    return (NULL);
                }
            }
        } else if (tokens->type == TOKEN_PIPE) {
            cmd->pipe_into = cmd_new(pool);
            cmd = cmd->pipe_into;
        } else if (tokens->type == TOKEN_LT) {
            if (!cmd_append_in_file(pool, cmd, tokens->value)) {
                return (NULL);
            }
        } else if (tokens->type == TOKEN_GT) {
            if (!cmd_append_out_file(pool, cmd, tokens->value)) {
                return (NULL);
            }
        } else {
            printf("Unknown token type\n");
            return (NULL);
        }
        tokens = tokens->next;
    }

    return (first);
}

void cmd_dump(t_cmd *cmd) {
    printf("program: %s\n", cmd->program);
    printf("args: [");
    for (size_t i = 0; cmd->args[i]; i++) {
        printf("%s, ", cmd->args[i]);
    }
    printf("]\n");
    if (cmd->in_files) {
        printf("in_files: [");
        for (size_t i = 0; cmd->in_files[i]; i++) {
            printf("%s, ", cmd->in_files[i]);
        }
        printf("]\n");
    }
    if (cmd->out_files) {
        printf("out_files: [");
        for (size_t i = 0; cmd->out_files[i]; i++) {
            printf("%s, ", cmd->out_files[i]);
        }
        printf("]\n");
    }
    if (cmd->pipe_into) {
        printf("pipe into\n");
        cmd_dump(cmd->pipe_into);
    }
}

int main(int argc, char **argv) {
    char *cmd_str = "cat \"hello.txt\" | cat";

    t_pool *parser_pool = pool_new();
    t_tokens *tokens = tokenize(parser_pool, cmd_str);
    if (!tokens) {
        pool_free(parser_pool);
        return (1);
    }

    // t_tokens *current = tokens;
    // while (current) {
    //     if (current->type == TOKEN_WORD) {
    //         printf("WORD: %s\n", current->value);
    //     } else if (current->type == TOKEN_PIPE) {
    //         printf("PIPE: |\n");
    //     } else {
    //         printf("UNKNOWN\n");
    //     }
    //     current = current->next;
    // }

    t_cmd *cmd = parse(parser_pool, tokens);
    if (!cmd) {
        pool_free(parser_pool);
        return (1);
    }

    // cmd_dump(cmd);

    pool_free(parser_pool);
    return (0);
}
