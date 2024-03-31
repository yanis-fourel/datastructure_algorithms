#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec32i.h"

#define FAIL                                                                   \
    eprintf("failure\n");                                                      \
    return 1;

#define eprintf(__format, ...)                                                 \
    {                                                                          \
        fprintf(stderr, "%d: ", __LINE__);                                     \
        if (errno) {                                                           \
            fprintf(stderr, "(errno %d: %s): ", errno, strerror(errno));       \
        }                                                                      \
        fprintf(stderr, __format __VA_OPT__(, ) __VA_ARGS__);                  \
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

int test_vec32_search() {
    t_vec32i *vec = vec32i_from({2, 1, 9, 0, 3});
    if (vec == NULL) {
        FAIL;
    }

    if (vec32i_search(vec, 2) != 0) {
        FAIL;
    }
    if (vec32i_search(vec, 1) != 1) {
        FAIL;
    }
    if (vec32i_search(vec, 9) != 2) {
        FAIL;
    }
    if (vec32i_search(vec, 0) != 3) {
        FAIL;
    }
    if (vec32i_search(vec, 3) != 4) {
        FAIL;
    }
    if (vec32i_search(vec, 4) != -1) {
        FAIL;
    }
    return 0;
}

// -------------------------------------------

#define RUN_TEST(func)                                                         \
    if (func() == 0) {                                                         \
        printf("OK - %s\n", #func);                                            \
        ++ok;                                                                  \
    } else {                                                                   \
        printf("KO - %s\n", #func);                                            \
        ++ko;                                                                  \
    }

int main(void) {
    int ok = 0;
    int ko = 0;

    RUN_TEST(test_vec32);
    RUN_TEST(test_vec32_search);

    printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
    return 0;
}
