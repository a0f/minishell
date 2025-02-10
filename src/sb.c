#include "sb.h"
#include "libft.h"
#include <string.h>

t_sb *sb_new(t_pool *pool) {
    t_sb *sb = pool_calloc(pool, sizeof(t_sb));
    sb->pool = pool;
    sb->data = pool_calloc(pool, 1);
    sb->length = 0;
    sb->capacity = 1;
    return sb;
}

void sb_free(t_sb *sb) {
    pool_dealloc(sb->pool, sb->data);
    pool_dealloc(sb->pool, sb);
}

// todo: put malloc checks in the append functions
void sb_append(t_sb *sb, const char *str) {
    size_t len = strlen(str);
    sb->capacity += len;
    sb->data = pool_realloc(sb->pool, sb->data, sb->capacity);
    ft_memcpy(sb->data + sb->length, str, len + 1);
    sb->length += len;
}

void sb_append_char(t_sb *sb, char c) {
    char str[2] = {c, 0};
    sb_append(sb, str);
}

char *sb_build(t_sb *sb) {
    char *data = sb->data;
    pool_dealloc(sb->pool, sb);
    return data;
}
