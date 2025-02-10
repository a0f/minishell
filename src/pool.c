#include "pool.h"
#include "libft.h"

t_pool *pool_new(void) {
    t_pool *pool = ft_calloc(1, sizeof(t_pool));
    if (!pool) {
        return NULL;
    }

    pool->capacity = 16;
    pool->ptrs = ft_calloc(pool->capacity, sizeof(void *));
    if (!pool->ptrs) {
        free(pool);
        return NULL;
    }

    return pool;
}

void pool_free(t_pool *pool) {
    for (size_t i = 0; i < pool->count; i++) {
        free(pool->ptrs[i]);
    }
    free(pool->ptrs);
    free(pool);
}

void *pool_calloc(t_pool *pool, size_t size) {
    void *ptr = ft_calloc(1, size);
    if (!ptr) {
        return NULL;
    }

    if (pool->count == pool->capacity) {
        size_t new_capacity = pool->capacity * 2;
        void **new_ptrs = realloc(pool->ptrs, new_capacity * sizeof(void *));
        if (!new_ptrs) {
            free(ptr);
            return NULL;
        }

        pool->capacity = new_capacity;
        pool->ptrs = new_ptrs;
    }

    pool->ptrs[pool->count++] = ptr;
    return ptr;
}

void *pool_realloc(t_pool *pool, void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        return NULL;
    }

    for (size_t i = 0; i < pool->count; i++) {
        if (pool->ptrs[i] == ptr) {
            pool->ptrs[i] = new_ptr;
            break;
        }
    }

    return new_ptr;
}

void pool_dealloc(t_pool *pool, void *ptr) {
    for (size_t i = 0; i < pool->count; i++) {
        if (pool->ptrs[i] == ptr) {
            free(ptr);
            for (size_t j = i; j < pool->count - 1; j++) {
                pool->ptrs[j] = pool->ptrs[j + 1];
            }
            pool->count--;
            break;
        }
    }
}
