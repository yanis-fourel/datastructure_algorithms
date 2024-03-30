#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_vec32i {
	int32_t *data;
	u_int32_t _size;
	u_int32_t _cap;
} t_vec32i;


t_vec32i *vec32i_new(u_int32_t capacity);
void vec32i_free(t_vec32i *vec);
t_vec32i *vec32i_append(t_vec32i *vec, int32_t n);
void vec32i_remove(t_vec32i *vec, size_t index);
void vec32i_print(t_vec32i *vec);
int test_vec32i();
