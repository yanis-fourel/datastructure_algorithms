#include "vec32i.h"

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
	res->_size = 0;
	return res;
}

void vec32i_free(t_vec32i *vec) {
	free(vec->data);
	free(vec);
}

// This potentially reallocates the data field, don't keep any reference to the
// old pointer
t_vec32i *vec32i_append(t_vec32i *vec, int32_t n) {
	if (vec->_size + 1 > vec->_cap) {
		vec->_cap *= 2;
		vec->data = realloc(vec->data, vec->_cap * sizeof(int32_t));
		if (vec->data == NULL) {
			return NULL;
		}
	}

	vec->data[vec->_size] = n;
	vec->_size += 1;
	return vec;
}

void vec32i_remove(t_vec32i *vec, size_t index) {
	if (index >= vec->_size) {
		fprintf(stderr, "Out of bound removal of index [%zu] on size %u\n", index, vec->_size);
		exit(EXIT_FAILURE);
	}
	for (size_t i = index; i < vec->_size; i++) {
		vec->data[i] = vec->data[i+1];
	}
	vec->_size -= 1;
}

void vec32i_print(t_vec32i *vec) {
	printf("<vec32i size=%3u, cap=%3u: [", vec->_size, vec->_cap);
	for (size_t i = 0; i < vec->_size; i++) {
		if (i != 0) {
			printf(", ");
		}
		printf("%d", vec->data[i]);
	}
	printf("]>\n");
}

int test_vec32i() {
	t_vec32i *myvec = vec32i_new(0);
	if (myvec == NULL) {
		perror("fail to create vec");
		return EXIT_FAILURE;
	}

	vec32i_print(myvec);

	for (int i = 0; i < 20; i++) {
		myvec = vec32i_append(myvec, i);
		if (myvec == NULL) {
			perror("fail to append");
			return EXIT_FAILURE;
		}
		vec32i_print(myvec);
	}

	vec32i_remove(myvec, 15);
	vec32i_print(myvec);

	vec32i_remove(myvec, 18);
	vec32i_print(myvec);

	vec32i_remove(myvec, 0);
	vec32i_print(myvec);

	vec32i_free(myvec);
}
