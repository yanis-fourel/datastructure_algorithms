#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

#define OOM_PANIC                                                              \
    {                                                                          \
        fprintf(stderr, "Panicked at %s:%d: errno %d: %s", __FILE__, __LINE__, \
                errno, strerror(errno));                                       \
        exit(EXIT_FAILURE);                                                    \
    }

void *malloc_or_panic(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        OOM_PANIC;
    }
    return p;
}

void *calloc_or_panic(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    if (p == NULL) {
        OOM_PANIC;
    }
    return p;
}

void *realloc_or_panic(void *ptr, size_t size) {
    void *p = realloc(ptr, size);
    if (p == NULL) {
        OOM_PANIC;
    }
    return p;
}
