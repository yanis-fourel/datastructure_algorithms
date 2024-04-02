/// TODO: explain why that works
/// layout is the same
/// static inline func can be redeclared

#pragma once // TOCHANGE

#include "vec.h"
#include <stdint.h>

typedef struct {
    int32_t *data;
    size_t __elsize;
    size_t size;
    size_t _cap;
} vec32i_t;

#define vec32i_from(...)                                                       \
    vec32i_from_buff((int32_t const *)(int32_t[])__VA_ARGS__,                  \
                     sizeof((int32_t[])__VA_ARGS__) / sizeof(int32_t))

static inline vec32i_t *vec32i_new(size_t size, size_t capacity) {
    return (vec32i_t *)vec_new(sizeof(int32_t), size, capacity);
}

static inline vec32i_t *vec32i_from_buff(void const *buff, size_t size) {
    return (vec32i_t *)vec_from_buff(buff, sizeof(int32_t), size);
}

static inline void vec32i_free(vec32i_t *vec) {
    vec_free((vec_t *)vec);
}

static inline void vec32i_append(vec32i_t *vec, int32_t val) {
    vec_append((vec_t *)vec, &val);
}

static inline void vec32i_remove(vec32i_t *vec, size_t index) {
    vec_remove((vec_t *)vec, index);
}

static inline bool vec32i_eq(vec32i_t const *a, vec32i_t const *b) {
    return vec_eq((vec_t const *)a, (vec_t const *)b);
}

static inline void vec32i_print(vec32i_t *vec) {
    vec_print((vec_t const *)vec);
}

static inline ssize_t vec32i_search(vec32i_t const *vec, int32_t val) {
    return vec_search((vec_t const *)vec, &val);
}

static inline ssize_t vec32i_search_binary(vec32i_t const *vec, int32_t val,
                                           int (*cmp)(void const *,
                                                      void const *)) {
    return vec_search_binary((vec_t const *)vec, &val, cmp);
}

static inline void vec32i_bubble_sort(vec32i_t const *vec,
                                      int (*cmp)(void const *, void const *)) {
    return vec_bubble_sort((vec_t const *)vec, cmp);
}
