///
/// Simple implementation of a growable array. It's just a contiguous block of
/// memory. Good.
///

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct s_vec32i {
    int32_t *data;
    u_int32_t size;
    u_int32_t _cap;
} vec32i_t;

#define vec32i_from(...)                                                       \
    vec32i_from_buff((int32_t const *)(int32_t[])__VA_ARGS__,                  \
                     sizeof((int32_t[])__VA_ARGS__) / sizeof(int32_t))

/// Returns an array of given size with all zero elements, and with given
/// capacity.
/// If size > capacity, then capacity = size
vec32i_t *vec32i_new(u_int32_t size, u_int32_t capacity);
vec32i_t *vec32i_from_buff(int32_t const *buff, size_t size);
void vec32i_free(vec32i_t *vec);

/// O(1)
/// Returns NULL if realloc failed, else return vec.
/// This potentially reallocates the data field, don't keep any reference to the
/// old vec->data pointer.
vec32i_t *vec32i_append(vec32i_t *vec, int32_t n);

/// O(1)
/// Removes given index
void vec32i_remove(vec32i_t *vec, size_t index);
void vec32i_print(vec32i_t *vec);
bool vec32i_eq(vec32i_t const *a, vec32i_t const *b);

/// O(n)
/// Returns the first index of a value or -1 if not found
ssize_t vec32i_search(vec32i_t const *vec, int32_t val);

/// O(log n)
/// Faster than regular search, but only works on sorted vectors
ssize_t vec32i_search_binary(vec32i_t const *vec, int32_t val);

/// O(sqrt n))
/// Very specialized question, only the truest will know
ssize_t vec32i_two_crystal_balls(vec32i_t const *vec);

/// O(n^2)
void vec32i_bubble_sort(vec32i_t const *vec);
