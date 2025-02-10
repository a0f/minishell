#pragma once

#include <stdlib.h>
#include "pool.h"

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
    t_pool *pool;
} t_sb;

t_sb *sb_new(t_pool *pool);

void sb_append(t_sb *sb, const char *str);
void sb_append_char(t_sb *sb, char c);
char *sb_build(t_sb *sb);
