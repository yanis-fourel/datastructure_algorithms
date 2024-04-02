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

#ifndef SKIP_VEC_IMPL

/// Here are all the basic vector type
/// You are encouraged to generated your own variations for your own data types

// TODO: do a bitmap, for the lolz
#define TYPE bool
#define NAME vecb
#include "_vec_impl.h"
#define vecb_from(...) (vecb_t *)vec_from(bool, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE char
#define NAME charvec
#include "_vec_impl.h"
#define charvec_from(...) (charvec_t *)vec_from(char, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE unsigned char
#define NAME ucharvec
#include "_vec_impl.h"
#define ucharvec_from(...) (ucharvec_t *)vec_from(unsigned char, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE int8_t
#define NAME i8vec
#include "_vec_impl.h"
#define i8vec_from(...) (i8vec_t *)vec_from(uint8_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE uint8_t
#define NAME u8vec
#include "_vec_impl.h"
#define u8vec_from(...) (u8vec_t *)vec_from(uint8_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE int16_t
#define NAME i16vec
#include "_vec_impl.h"
#define i16vec_from(...) (i16vec_t *)vec_from(int16_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE uint16_t
#define NAME u16vec
#include "_vec_impl.h"
#define u16vec_from(...) (u16vec_t *)vec_from(uint16_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE int32_t
#define NAME i32vec
#include "_vec_impl.h"
#define i32vec_from(...) (i32vec_t *)vec_from(int32_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE uint32_t
#define NAME u32vec
#include "_vec_impl.h"
#define u32vec_from(...) (u32vec_t *)vec_from(uint32_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE int64_t
#define NAME i64vec
#include "_vec_impl.h"
#define i64vec_from(...) (i64vec_t *)vec_from(int64_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE uint64_t
#define NAME u64vec
#include "_vec_impl.h"
#define u64vec_from(...) (u64vec_t *)vec_from(uint64_t, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE float
#define NAME fvec
#include "_vec_impl.h"
#define fvec_from(...) (fvec_t *)vec_from(float, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE double
#define NAME dvec
#include "_vec_impl.h"
#define dvec_from(...) (dvec_t *)vec_from(double, __VA_ARGS__)
#undef TYPE
#undef NAME

#define TYPE long double
#define NAME ldvec
#include "_vec_impl.h"
#define ldvec_from(...) (ldvec_t *)vec_from(long double, __VA_ARGS__)
#undef TYPE
#undef NAME

#endif
