///
/// Custom allocator that panic in case of allocation failure, that way we don't
/// need to handle the error case.
/// Until I actually encouter a case where I am able to recover from an out of
/// memory error, I don't want to carry the burden of bubbling up memory
/// allocation errors.
///

#pragma once

#include <sys/types.h>

/// Same as malloc(2) but panics in case of allocation error
void *malloc_or_panic(size_t size) __attribute_malloc__
    __attribute_alloc_size__((1)) __wur;

/// Same as calloc(2) but panics in case of allocation error
void *calloc_or_panic(size_t nmemb, size_t size) __attribute_malloc__
    __attribute_alloc_size__((1, 2)) __wur;

/// Same as realloc(2) but panics in case of allocation error
void *realloc_or_panic(void *ptr, size_t size) __attribute_warn_unused_result__
    __attribute_alloc_size__((2));
