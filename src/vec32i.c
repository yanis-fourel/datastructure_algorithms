#include "vec32i.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

t_vec32i *vec32i_new(u_int32_t capacity) {
    t_vec32i *res = malloc(sizeof(t_vec32i));
    if (res == NULL) {
        return NULL;
    }

    if (capacity < 8) {
        capacity = 8;
    }
    res->data = malloc(capacity * sizeof(int32_t));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    res->_cap = capacity;
    res->size = 0;
    return res;
}

t_vec32i *vec32i_from_buff(int32_t const *buff, size_t size) {
    t_vec32i *res = vec32i_new(size);
    if (res == NULL) {
        return NULL;
    }
    memcpy(res->data, buff, size);
    res->size = size;
    return res;
}

void vec32i_free(t_vec32i *vec) {
    free(vec->data);
    free(vec);
}

// This potentially reallocates the data field, don't keep any reference to the
// old pointer
t_vec32i *vec32i_append(t_vec32i *vec, int32_t n) {
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

void vec32i_remove(t_vec32i *vec, size_t index) {
    if (index >= vec->size) {
        fprintf(stderr, "Out of bound removal of index [%zu] on size %u\n",
                index, vec->size);
        exit(EXIT_FAILURE);
    }
    for (size_t i = index; i < vec->size; i++) {
        vec->data[i] = vec->data[i + 1];
    }
    vec->size -= 1;
}

bool vec32i_eq(t_vec32i const *a, t_vec32i const *b) {
    if (a->size != b->size) {
        return false;
    }
    return memcmp(a->data, b->data, a->size) == 0;
}

void vec32i_print(t_vec32i *vec) {
    printf("<vec32i size=%3u, cap=%3u: [", vec->size, vec->_cap);
    for (size_t i = 0; i < vec->size; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", vec->data[i]);
    }
    printf("]>\n");
}
