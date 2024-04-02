/// TODO: explain why that works
/// layout is the same
/// static inline func can be redeclared

// NO INCLUDE GUARD

#ifndef TYPE
// Note: This is just for my linter to understand this file
#    define TYPE int32_t
#    define NAME vec32i
#    define SKIP_VEC_IMPL
#    include "vec.h"
#    undef SKIP_VEC_IMPL
#endif

#include <stdint.h>
#include <sys/types.h>

#define CONCAT_EVAL(a, b) CONCAT(a, b)
#define CONCAT(a, b) a##b
#define _NAME(suffix) CONCAT_EVAL(NAME, suffix)

typedef struct {
    TYPE *data;
    size_t __elsize;
    size_t size;
    size_t _cap;
} _NAME(_t);

#define XSTR(x) STR(x)
#define STR(x) #x

static inline _NAME(_t) * _NAME(_new)(size_t size, size_t capacity) {
    return (_NAME(_t) *)vec_new(sizeof(TYPE), size, capacity);
}

static inline _NAME(_t) * _NAME(_from_buff)(const void *buff, size_t size) {
    return (_NAME(_t) *)vec_from_buff(buff, sizeof(TYPE), size);
}

static inline void _NAME(_free)(_NAME(_t) * vec) {
    vec_free((vec_t *)vec);
}

static inline void _NAME(_append)(_NAME(_t) * vec, TYPE val) {
    vec_append((vec_t *)vec, &val);
}

static inline void _NAME(_remove)(_NAME(_t) * vec, size_t index) {
    vec_remove((vec_t *)vec, index);
}

static inline bool _NAME(_eq)(_NAME(_t) const *a, _NAME(_t) const *b) {
    return vec_eq((const vec_t *)a, (const vec_t *)b);
}

static inline void _NAME(_print)(_NAME(_t) * vec) {
    vec_print((const vec_t *)vec);
}

static inline ssize_t _NAME(_search)(_NAME(_t) const *vec, TYPE val) {
    return vec_search((const vec_t *)vec, &val);
}

static inline ssize_t
_NAME(_search_binary)(_NAME(_t) const *vec, TYPE val,
                      int (*cmp)(const void *, const void *)) {
    return vec_search_binary((const vec_t *)vec, &val, cmp);
}

static inline void _NAME(_bubble_sort)(_NAME(_t) const *vec,
                                       int (*cmp)(const void *, const void *)) {
    return vec_bubble_sort((const vec_t *)vec, cmp);
}
