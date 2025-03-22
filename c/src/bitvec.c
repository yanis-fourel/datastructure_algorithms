#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "vec.h"

static size_t _to_byte_size(size_t size) {
    return (size / 8) + (size % 8 != 0);
}

/// Allocate more size if needed to match newsize, appending zeros
static void _increase_size(bitvec_t *vec, size_t newsize) {
    size_t newcap = vec->_cap;
    while (newcap < newsize) {
        newcap *= 2;
    }
    if (newcap == vec->_cap) {
        if (newsize > vec->size) {
            vec->size = newsize;
        }
        return;
    }

    vec->_data = realloc_or_panic(vec->_data, newcap);
    memset(vec->_data + vec->_cap, 0, newcap - vec->_cap);
    vec->_cap = newcap;
    vec->size = newsize;
}

bitvec_t *bitvec_new(size_t size, size_t capacity) {
    bitvec_t *res = malloc(sizeof(bitvec_t));

    if (_to_byte_size(size) > capacity) {
        capacity = _to_byte_size(size);
    }
    if (capacity < 8) {
        capacity = 8;
    }
    // Important: Any bits allocated but not in use should be zeroed
    res->_data = calloc_or_panic(capacity, 1);

    res->_cap = capacity;
    res->size = size;
    return res;
}

bitvec_t *bitvec_from_buff(const bool *buff, size_t size) {
    bitvec_t *res = bitvec_new(0, size);
    for (size_t i = 0; i < size; ++i) {
        bitvec_set(res, i, buff[i]);
    }
    res->size = size;
    return res;
}

void bitvec_free(bitvec_t *vec) {
    vec_free((vec_t *)vec);
}

void bitvec_set(bitvec_t *vec, size_t index, bool val) {
    if (index >= vec->size) {
        _increase_size(vec, index + 1);
    }
    uint8_t *byte = vec->_data + index / 8;
    uint8_t bit = index % 8;

    if (val) {
        *byte |= 1 << bit;
    } else {
        *byte &= ~(1 << bit);
    }
}

bool bitvec_get(const bitvec_t *vec, size_t index) {
    if (index >= vec->size) {
        return false;
    }
    uint8_t _byte = ((uint8_t *)vec->_data)[index / 8];
    uint8_t sbyte = _byte >> (index % 8);
    return sbyte & 1;
}

void bitvec_append(bitvec_t *vec, bool val) {
    bitvec_set(vec, vec->size, val);
}

void bitvec_remove(bitvec_t *vec, size_t index) {
    if (vec->size < index) {
        fprintf(stderr, "Out of bound removal of index [%zu] on size %zu\n",
                index, vec->size);
        exit(EXIT_FAILURE);
    }

    for (; index < vec->size; ++index) {
        bitvec_set(vec, index, bitvec_get(vec, index + 1));
    }
    --vec->size;
}

bool bitvec_eq(const bitvec_t *a, const bitvec_t *b) {
    if (a->size != b->size) {
        return false;
    }
    return memcmp(a->_data, b->_data, _to_byte_size(a->size)) == 0;
}

void bitvec_print(const bitvec_t *vec) {
    printf("<bitvec size=%3zu entries, cap=%3zu bytes: [", vec->size,
           vec->_cap);
    for (size_t i = 0; i < vec->size; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", bitvec_get(vec, i));
    }
    printf("]>\n");
}

ssize_t bitvec_search(const bitvec_t *vec, bool val) {
    for (size_t i = 0; i < vec->size; ++i) {
        if (bitvec_get(vec, i) == val) {
            return (ssize_t)i;
        }
    }
    return -1;
}

ssize_t bitvec_two_crystal_balls(const bitvec_t *vec) {
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
            if (bitvec_get(vec, vec->size - 1) == 0) {
                return -1;
            }
            break;
        }
        if (bitvec_get(vec, i)) {
            break;
        }
    }
    for (size_t j = (i - step);; ++j) {
        if (bitvec_get(vec, j)) {
            return j;
        }
    }
}
