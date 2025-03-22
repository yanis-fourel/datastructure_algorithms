#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "alloc.h"
#include "vec.h"

uint8_t *ptrat(const vec_t *vec, uint8_t elsize, size_t idx) {
    return &vec->data[idx * elsize];
}

vec_t *vec_new(size_t elsize, size_t size, size_t capacity) {
    vec_t *res = malloc_or_panic(sizeof(vec_t));

    if (size > capacity) {
        capacity = size;
    }
    if (capacity < 8) {
        capacity = 8;
    }
    res->data = calloc_or_panic(capacity, elsize);

    res->_cap = capacity;
    res->size = size;
    return res;
}

vec_t *vec_from_buff(const void *buff, size_t elsize, size_t size) {
    vec_t *res = vec_new(elsize, 0, size);
    memcpy(res->data, buff, size * elsize);
    res->size = size;
    return res;
}

void vec_free(vec_t *vec) {
    free(vec->data);
    free(vec);
}

void vec_append(vec_t *vec, uint8_t elsize, const void *data) {
    if (vec->size == vec->_cap) {
        vec->_cap *= 2;
        vec->data = realloc_or_panic(vec->data, vec->_cap * elsize);
    }

    memcpy(ptrat(vec, elsize, vec->size), data, elsize);
    ++vec->size;
}

void vec_remove(vec_t *vec, uint8_t elsize, size_t index) {
    if (vec->size < index) {
        fprintf(stderr, "Out of bound removal of index [%zu] on size %zu\n",
                index, vec->size);
        exit(EXIT_FAILURE);
    }
    for (size_t i = index * elsize; i < vec->size * elsize; ++i) {
        ((uint8_t *)vec->data)[i] = ((uint8_t *)vec->data)[i + elsize];
    }
    vec->size -= 1;
}

bool vec_eq(const vec_t *a, const vec_t *b) {
    if (a->size != b->size) {
        return false;
    }
    return memcmp(a->data, b->data, a->size) == 0;
}

void vec_print(const vec_t *vec, uint8_t elsize) {
    printf("<vec size=%3zu, cap=%3zu: [", vec->size, vec->_cap);
    for (size_t i = 0; i < vec->size * elsize; ++i) {
        if (i != 0) {
            if (i % elsize) {
                printf(" ");
            } else {
                printf(", ");
            }
        }
        printf("%2x", ((uint8_t *)vec->data)[i]);
    }
    printf("]>\n");
}

ssize_t vec_search(const vec_t *vec, uint8_t elsize, const void *val) {
    for (size_t i = 0; i < vec->size; ++i) {
        if (memcmp(ptrat(vec, elsize, i), val, elsize) == 0) {
            return (ssize_t)i;
        }
    }
    return -1;
}

ssize_t vec_search_binary(const vec_t *vec, uint8_t elsize, const void *val,
                          int (*cmp)(const void *, const void *)) {
    size_t lo = 0;
    size_t hi = vec->size;

    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        int c = cmp(ptrat(vec, elsize, mid), val);
        if (c == 0) {
            return mid;
        } else if (c < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}

void vec_bubble_sort(const vec_t *vec, uint8_t elsize,
                     int (*cmp)(const void *, const void *)) {
    void *tmp = malloc_or_panic(elsize);
    for (size_t i = 0; i < vec->size; ++i) {
        for (size_t j = 0; j < vec->size - i - 1; ++j) {
            if (cmp(ptrat(vec, elsize, j), ptrat(vec, elsize, j + 1)) > 0) {
                memcpy(tmp, ptrat(vec, elsize, j), elsize);
                memcpy(ptrat(vec, elsize, j), ptrat(vec, elsize, j + 1),
                       elsize);
                memcpy(ptrat(vec, elsize, j + 1), tmp, elsize);
            }
        }
    }
}
