#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "vec32i.h"

vec32i_t *vec32i_new(u_int32_t size, u_int32_t capacity) {
    vec32i_t *res = malloc(sizeof(vec32i_t));
    if (res == NULL) {
        return NULL;
    }

    if (size > capacity) {
        capacity = size;
    }
    if (capacity < 8) {
        capacity = 8;
    }
    res->data = calloc(capacity, sizeof(int32_t));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    res->_cap = capacity;
    res->size = size;
    return res;
}

vec32i_t *vec32i_from_buff(int32_t const *buff, size_t size) {
    vec32i_t *res = vec32i_new(0, size);
    if (res == NULL) {
        return NULL;
    }
    memcpy(res->data, buff, size * sizeof(int32_t));
    res->size = size;
    return res;
}

void vec32i_free(vec32i_t *vec) {
    free(vec->data);
    free(vec);
}

vec32i_t *vec32i_append(vec32i_t *vec, int32_t n) {
    if (vec->size + 1 > vec->_cap) {
        vec->_cap *= 2;
        vec->data = realloc(vec->data, vec->_cap * sizeof(int32_t));
        if (vec->data == NULL) {
            return NULL;
        }
    }

    vec->data[vec->size] = n;
    vec->size += 1;
    return vec;
}

void vec32i_remove(vec32i_t *vec, size_t index) {
    if (index >= vec->size) {
        fprintf(stderr, "Out of bound removal of index [%zu] on size %u\n",
                index, vec->size);
        exit(EXIT_FAILURE);
    }
    for (size_t i = index; i < vec->size; ++i) {
        vec->data[i] = vec->data[i + 1];
    }
    vec->size -= 1;
}

bool vec32i_eq(vec32i_t const *a, vec32i_t const *b) {
    if (a->size != b->size) {
        return false;
    }
    return memcmp(a->data, b->data, a->size * sizeof(int32_t)) == 0;
}

void vec32i_print(vec32i_t *vec) {
    printf("<vec32i size=%3u, cap=%3u: [", vec->size, vec->_cap);
    for (size_t i = 0; i < vec->size; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", vec->data[i]);
    }
    printf("]>\n");
}

ssize_t vec32i_search(vec32i_t const *vec, int32_t val) {
    for (size_t i = 0; i < vec->size; ++i) {
        if (vec->data[i] == val) {
            return i;
        }
    }
    return -1;
}

ssize_t vec32i_search_binary(vec32i_t const *vec, int32_t val) {
    size_t lo = 0;
    size_t hi = vec->size;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (vec->data[mid] == val) {
            return mid;
        } else if (vec->data[mid] < val) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}

ssize_t vec32i_two_crystal_balls(vec32i_t const *vec) {
    if (vec->size == 0) {
        return -1;
    }

    size_t step = sqrt(vec->size);
    if (step < 2) {
        step = 2;
    }
    size_t i = step;
    for (;; i += step) {
        if (i >= vec->size) {
            if (vec->data[vec->size - 1] == 0) {
                return -1;
            }
            break;
        }
        if (vec->data[i]) {
            break;
        }
    }
    for (size_t j = (i - step);; ++j) {
        if (vec->data[j]) {
            return j;
        }
    }
}

void vec32i_bubble_sort(vec32i_t const *vec) {
    int32_t tmp;

    for (size_t i = 0; i < vec->size; ++i) {
        for (size_t j = 0; j < vec->size - i - 1; ++j) {
            if (vec->data[j] > vec->data[j + 1]) {
                tmp = vec->data[j];
                vec->data[j] = vec->data[j + 1];
                vec->data[j + 1] = tmp;
            }
        }
    }
}
