#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "alloc.h"
#include "vec.h"

uint8_t *ptrat(vec_t const *vec, size_t idx) {
    return &vec->data[idx * vec->elsize];
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
    res->elsize = elsize;
    return res;
}

vec_t *vec_from_buff(void const *buff, size_t elsize, size_t size) {
    vec_t *res = vec_new(elsize, 0, size);
    memcpy(res->data, buff, size * elsize);
    res->size = size;
    return res;
}

void vec_free(vec_t *vec) {
    free(vec->data);
    free(vec);
}

void vec_append(vec_t *vec, void const *data) {
    if (vec->size == vec->_cap) {
        vec->_cap *= 2;
        vec->data = realloc_or_panic(vec->data, vec->_cap * vec->elsize);
    }

    memcpy(ptrat(vec, vec->size), data, vec->elsize);
    ++vec->size;
}

void vec_remove(vec_t *vec, size_t index) {
    if (vec->size < index) {
        fprintf(stderr, "Out of bound removal of index [%zu] on size %zu\n",
                index, vec->size);
        exit(EXIT_FAILURE);
    }
    for (size_t i = index * vec->elsize; i < vec->size * vec->elsize; ++i) {
        ((uint8_t *)vec->data)[i] = ((uint8_t *)vec->data)[i + vec->elsize];
    }
    vec->size -= 1;
}

bool vec_eq(vec_t const *a, vec_t const *b) {
    if (a->size != b->size) {
        return false;
    }
    return memcmp(a->data, b->data, a->size) == 0;
}

void vec_print(vec_t const *vec) {
    printf("<vec size=%3zu, cap=%3zu, elsize=%3zu: [", vec->size, vec->_cap,
           vec->elsize);
    for (size_t i = 0; i < vec->size * vec->elsize; ++i) {
        if (i != 0) {
            if (i % vec->elsize) {
                printf(" ");
            } else {
                printf(", ");
            }
        }
        printf("%2x", ((uint8_t *)vec->data)[i]);
    }
    printf("]>\n");
}

ssize_t vec_search(vec_t const *vec, void const *val) {
    for (size_t i = 0; i < vec->size; ++i) {
        if (memcmp(ptrat(vec, i), val, vec->elsize) == 0) {
            return i;
        }
    }
    return -1;
}

ssize_t vec_search_binary(vec_t const *vec, void const *val,
                          int (*cmp)(void const *, void const *)) {
    size_t lo = 0;
    size_t hi = vec->size;

    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        int c = cmp(ptrat(vec, mid), val);
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

// TODO: only implement for vec_bool
// ssize_t vec_two_crystal_balls(vec_t const *vec) {
//     if (vec->size == 0) {
//         return -1;
//     }
//
//     uint8_t const *data = (uint8_t const *)vec->data;
//     size_t step = sqrt(vec->size);
//     if (step < 2) {
//         step = 2;
//     }
//     size_t i = step;
//     for (;; i += step) {
//         if (i >= vec->size) {
//             if (data[vec->size - 1] == 0) {
//                 return -1;
//             }
//             break;
//         }
//         if (data[i]) {
//             break;
//         }
//     }
//     for (size_t j = (i - step);; ++j) {
//         if (data[j]) {
//             return j;
//         }
//     }
// }

void vec_bubble_sort(vec_t const *vec, int (*cmp)(void const *, void const *)) {
    void *tmp = malloc_or_panic(vec->elsize);
    for (size_t i = 0; i < vec->size; ++i) {
        for (size_t j = 0; j < vec->size - i - 1; ++j) {
            if (cmp(ptrat(vec, j), ptrat(vec, j + 1)) > 0) {
                memcpy(tmp, ptrat(vec, j), vec->elsize);
                memcpy(ptrat(vec, j), ptrat(vec, j + 1), vec->elsize);
                memcpy(ptrat(vec, j + 1), tmp, vec->elsize);
            }
        }
    }
}
