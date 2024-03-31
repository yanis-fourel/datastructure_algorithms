#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct s_vec32i {
	int32_t *data;
	u_int32_t size;
	u_int32_t _cap;
} t_vec32i;


#define vec32i_from(...) vec32i_from_buff((int32_t const*)(int32_t []) __VA_ARGS__, sizeof((int32_t []) __VA_ARGS__)/sizeof(int32_t))


t_vec32i *vec32i_new(u_int32_t capacity);
t_vec32i *vec32i_from_buff(int32_t const* buff, size_t size);
void vec32i_free(t_vec32i *vec);
t_vec32i *vec32i_append(t_vec32i *vec, int32_t n);
void vec32i_remove(t_vec32i *vec, size_t index);
void vec32i_print(t_vec32i *vec);
bool vec32i_eq(t_vec32i const* a, t_vec32i const *b);
