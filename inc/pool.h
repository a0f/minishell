#pragma once

#include <stdlib.h>

typedef struct {
    void **ptrs;
    size_t count;
    size_t capacity;
} t_pool;

t_pool *pool_new(void);
void pool_free(t_pool *pool);

void *pool_calloc(t_pool *pool, size_t size);
void *pool_realloc(t_pool *pool, void *ptr, size_t size);
void pool_dealloc(t_pool *pool, void *ptr);
