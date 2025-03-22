#pragma once

#include <stdbool.h>
#include <sys/types.h>

typedef struct {
    // Use `bitvec_get` and `bitvec_set` to interract with the data
    // This is a bitmap where each byte contains 8 entries, written from least
    // significant to most significant.
    // So index 0 is at 0b00000001, index 1 at 0b00000010 etc...
    // Index 8 is at 0b00000001 on the second byte etc...
    // size can be any arbitrary number, and if it's not a multiple of 8 it is
    // guaranteed that all extra bits of the last byte are set to 0
    void *_data;
    // Size in number of elements
    size_t size;
    // Capacity in bytes
    size_t _cap;
} bitvec_t;

#define bitvec_from(...)                                                       \
    bitvec_from_buff((bool const *)(bool[])__VA_ARGS__,                        \
                     sizeof((bool[])__VA_ARGS__) / sizeof(bool))

/// Returns an array of given size with all zero elements, and with given
/// capacity.
/// If size > capacity, then capacity = size
/// Always return a valid pointer. Panics in case of allocation error.
bitvec_t *bitvec_new(size_t size, size_t capacity);

/// Always return a valid pointer. Panics in case of allocation error.
bitvec_t *bitvec_from_buff(const bool *buff, size_t size);

void bitvec_free(bitvec_t *vec);

/// O(1)
/// If the index is out of bound, it will extend the vector with zeros until
/// it can set the given index
/// This potentially reallocates the data field, don't keep any reference to
/// the old vec->data pointer.
void bitvec_set(bitvec_t *vec, size_t index, bool val);

/// O(1)
/// If the index is out of bound, return false
bool bitvec_get(const bitvec_t *vec, size_t index);

/// O(1)
/// This potentially reallocates the data field, don't keep any reference to
/// the old vec->data pointer.
void bitvec_append(bitvec_t *vec, bool val);

/// O(1)
/// Removes given index
/// Panics if the index is out of bound
void bitvec_remove(bitvec_t *vec, size_t index);

bool bitvec_eq(const bitvec_t *a, const bitvec_t *b);

void bitvec_print(const bitvec_t *vec);

/// O(n)
/// Returns the first index of a value or -1 if not found
ssize_t bitvec_search(const bitvec_t *vec, bool val);

/// O(sqrt n)t
/// Very specialized question, only the truest will know
ssize_t bitvec_two_crystal_balls(const bitvec_t *vec);
