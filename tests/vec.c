#include "vec32i.h"
#include <stdio.h>

int test_vec32_simple() {
	t_vec32i *myvec = vec32i_new(0);
	if (myvec == NULL) {
		printf("myvec is null");
		return 1;
	}
	vec32i_free(myvec);

	return 0;
}

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

	RUN_TEST(test_vec32_simple);

	printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
	return 0;
}
