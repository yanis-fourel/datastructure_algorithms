#include "vec32i.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define FAIL eprintf("failure\n"); return 1;

int eprintf(const char *__restrict __format, ...) __nonnull ((1)) {
	int res = 0;
	res += fprintf(stderr, "%d: ", __LINE__);
	if (errno) {
		res += fprintf(stderr, "(errno %d: %s): ", errno, strerror(errno));
	}
	va_list ptr;
	va_start(ptr, __format);
	res += vfprintf(stderr, __format, ptr);
	return res;
}


int test_vec32() {
	t_vec32i *myvec = vec32i_new(0);
	if (myvec == NULL) {
		eprintf("myvec is null");
		return 1;
	}

	if (myvec->size != 0) {
		FAIL;
	}

	{
		t_vec32i *tmp = vec32i_from({});
		if (!vec32i_eq(myvec, tmp)) {
			FAIL;
		}
		vec32i_free(tmp);
	}

	for (int i = 0; i < 10; i++) {
		myvec = vec32i_append(myvec, i);
		if (myvec == NULL) {
			FAIL;
		}
	}

	vec32i_remove(myvec, 7);
	{
		t_vec32i *tmp = vec32i_from({0, 1, 2, 3, 4, 5, 6, 8, 9});
			if (!vec32i_eq(myvec, tmp)) {
				FAIL;
			}
			vec32i_free(tmp);
							  }

							  vec32i_remove(myvec, 8);
		{
			t_vec32i *tmp = vec32i_from({0, 1, 2, 3, 4, 5, 6, 8});
			if (!vec32i_eq(myvec, tmp)) {
				FAIL;
			}
			vec32i_free(tmp);
		}

		vec32i_remove(myvec, 0);
		{
			t_vec32i *tmp = vec32i_from({1, 2, 3, 4, 5, 6, 8});
			if (!vec32i_eq(myvec, tmp)) {
				FAIL;
			}
			vec32i_free(tmp);
		}

		for (int i = 0; i < 6; i++) {
			vec32i_remove(myvec, 1);
		}
		{
			t_vec32i *tmp = vec32i_from({1});
			if (!vec32i_eq(myvec, tmp)) {
				FAIL;
			}
			vec32i_free(tmp);
		}

		vec32i_remove(myvec, 0);
		{
			t_vec32i *tmp = vec32i_from({});
			if (!vec32i_eq(myvec, tmp)) {
				FAIL;
			}
			vec32i_free(tmp);
		}

		vec32i_free(myvec);

		return 0;
	}

	// -------------------------------------------

#define RUN_TEST(func) \
	if (func() == 0) { \
		 printf("OK - %s\n", #func); \
		  ++ok; \
		  } else { \
		  printf("KO - %s\n", #func); \
		  ++ko; \
		  }

	int main(void)
	{
		int ok = 0;
		int ko = 0;

		RUN_TEST(test_vec32);

		printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
		return 0;
	}
