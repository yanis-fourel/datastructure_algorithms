///
/// Simple implementation of a growable array. It's just a contiguous block of
/// memory. Good.
///

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct {
    uint8_t *data;

    // Size of each element
    size_t elsize;

    // Number of element
    size_t size;

    // Maximum number of element we can have without reallocating
    size_t _cap;
} vec_t;

#define vec_from(type, ...)                                                    \
    vec_from_buff((type const *)(type[])__VA_ARGS__, sizeof(type),             \
                  sizeof((int32_t[])__VA_ARGS__) / sizeof(type))

/// Returns an array of given size with all zero elements, and with given
/// capacity.
/// If size > capacity, then capacity = size
/// Always return a valid pointer. Panics in case of allocation error.
vec_t *vec_new(size_t elsize, size_t size, size_t capacity);

/// Always return a valid pointer. Panics in case of allocation error.
vec_t *vec_from_buff(void const *buff, size_t elsize, size_t size);

void vec_free(vec_t *vec);

/// O(1)
/// This potentially reallocates the data field, don't keep any reference to
/// the old vec->data pointer.
void vec_append(vec_t *vec, void const *data);

/// O(1)
/// Removes given index
void vec_remove(vec_t *vec, size_t index);

/// Two vec are equal if their size and data are equal, even if the capacity or
/// element isze are different
bool vec_eq(vec_t const *a, vec_t const *b);

void vec_print(vec_t const *vec);

/// O(n)
/// Returns the first index of a value or -1 if not found
ssize_t vec_search(vec_t const *vec, void const *val);

/// O(log n)
/// Faster than regular search, but only works on sorted vectors
ssize_t vec_search_binary(vec_t const *vec, void const *val,
                          int (*cmp)(void const *, void const *));

/// O(sqrt n)t
/// Very specialized question, only the truest will know
// ssize_t vec_two_crystal_balls(vec_t const *vec);

/// O(n^2)
void vec_bubble_sort(vec_t const *vec, int (*cmp)(void const *, void const *));

#include "vec_impl.h"
