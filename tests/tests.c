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

int test_vec32i() {
    t_vec32i *myvec = vec32i_new(0, 0);
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

int test_vec32i_search() {
    t_vec32i *vec = vec32i_from({2, 1, -1, 9, -6, 0, 3});
    if (vec == NULL) {
        FAIL;
    }

    if (vec32i_search(vec, 2) != 0) {
        FAIL;
    }
    if (vec32i_search(vec, 1) != 1) {
        FAIL;
    }
    if (vec32i_search(vec, -1) != 2) {
        FAIL;
    }
    if (vec32i_search(vec, 9) != 3) {
        FAIL;
    }
    if (vec32i_search(vec, -6) != 4) {
        FAIL;
    }
    if (vec32i_search(vec, 0) != 5) {
        FAIL;
    }
    if (vec32i_search(vec, 3) != 6) {
        FAIL;
    }
    if (vec32i_search(vec, 4) != -1) {
        FAIL;
    }
    if (vec32i_search(vec, -2) != -1) {
        FAIL;
    }

    vec32i_free(vec);
    return 0;
}

int test_vec32i_binary_search() {
    t_vec32i *vec = vec32i_from({-32, -6, 0, 1, 3, 7, 32, 1024});

    if (vec == NULL) {
        FAIL;
    }

    if (vec32i_search_binary(vec, -32) != 0) {
        FAIL;
    }
    if (vec32i_search_binary(vec, -6) != 1) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 0) != 2) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 1) != 3) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 3) != 4) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 7) != 5) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 32) != 6) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 1024) != 7) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 89) != -1) {
        FAIL;
    }
    if (vec32i_search_binary(vec, -1) != -1) {
        FAIL;
    }
    if (vec32i_search_binary(vec, 9000) != -1) {
        FAIL;
    }
    if (vec32i_search_binary(vec, -9000) != -1) {
        FAIL;
    }

    vec32i_free(vec);
    return 0;
}

t_vec32i *create_two_crystal_balls_input(size_t size, ssize_t answer) {
    t_vec32i *vec = vec32i_new(size, size);
    if (vec == NULL) {
        return NULL;
    }

    if (answer >= 0) {
        for (; answer < size; ++answer) {
            vec->data[answer] = 1;
        }
    }
    return vec;
}
int test_vec32i_two_crystal_balls() {
    {
        ssize_t answer = 234;
        t_vec32i *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 0;
        t_vec32i *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 999;
        t_vec32i *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = -1;
        t_vec32i *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = -1;
        t_vec32i *vec = create_two_crystal_balls_input(0, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 0;
        t_vec32i *vec = create_two_crystal_balls_input(1, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 5;
        t_vec32i *vec = create_two_crystal_balls_input(6, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
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

    RUN_TEST(test_vec32i);
    RUN_TEST(test_vec32i_search);
    RUN_TEST(test_vec32i_binary_search);
    RUN_TEST(test_vec32i_two_crystal_balls);

    printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
    return 0;
}
