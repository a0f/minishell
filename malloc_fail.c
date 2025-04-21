#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

typedef void *(*malloc_type)(size_t);

void *malloc(size_t size)
{
	static size_t count = 0;
	count += 1;

    static malloc_type orig_malloc = NULL;
    if (!orig_malloc)
    {
		orig_malloc = (malloc_type)dlsym(RTLD_NEXT, "malloc");
        if (!orig_malloc)
        {
			fprintf(stderr, "Error in dlsym: %s\n", dlerror());
            return NULL;
        }
    }
    if (count == 1500)
		return (NULL);
    return orig_malloc(size);
}
