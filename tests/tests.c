#include <errno.h>
#include <stdio.h>
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
    vec32i_t *myvec = vec32i_new(0, 0);

    if (myvec->size != 0) {
        FAIL;
    }

    {
        vec32i_t *tmp = vec32i_from({});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    for (int i = 0; i < 10; i++) {
        vec32i_append(myvec, i);
    }

    vec32i_remove(myvec, 7);
    {
        vec32i_t *tmp = vec32i_from({0, 1, 2, 3, 4, 5, 6, 8, 9});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    vec32i_remove(myvec, 8);
    {
        vec32i_t *tmp = vec32i_from({0, 1, 2, 3, 4, 5, 6, 8});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    vec32i_remove(myvec, 0);
    {
        vec32i_t *tmp = vec32i_from({1, 2, 3, 4, 5, 6, 8});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    for (int i = 0; i < 6; i++) {
        vec32i_remove(myvec, 1);
    }
    {
        vec32i_t *tmp = vec32i_from({1});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    vec32i_remove(myvec, 0);
    {
        vec32i_t *tmp = vec32i_from({});
        if (!vec32i_eq(myvec, tmp)) {
            FAIL;
        }
        vec32i_free(tmp);
    }

    vec32i_free(myvec);

    return 0;
}

int test_vec32i_search() {
    vec32i_t *vec = vec32i_from({2, 1, -1, 9, -6, 0, 3});

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
    vec32i_t *vec = vec32i_from({-32, -6, 0, 1, 3, 7, 32, 1024});

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

vec32i_t *create_two_crystal_balls_input(size_t size, ssize_t answer) {
    vec32i_t *vec = vec32i_new(size, size);

    if (answer >= 0) {
        for (; (size_t)answer < size; ++answer) {
            vec->data[answer] = 1;
        }
    }
    return vec;
}
int test_vec32i_two_crystal_balls() {
    {
        ssize_t answer = 234;
        vec32i_t *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 0;
        vec32i_t *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 999;
        vec32i_t *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = -1;
        vec32i_t *vec = create_two_crystal_balls_input(1000, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = -1;
        vec32i_t *vec = create_two_crystal_balls_input(0, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 0;
        vec32i_t *vec = create_two_crystal_balls_input(1, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    {
        ssize_t answer = 5;
        vec32i_t *vec = create_two_crystal_balls_input(6, answer);
        if (vec32i_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        vec32i_free(vec);
    }
    return 0;
}

int test_vec32i_bubble_sort() {
    {
        vec32i_t *vec = vec32i_from({1, 0, 3});
        vec32i_t *answer = vec32i_from({0, 1, 3});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            vec32i_print(vec);
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({-2, 0, 1, 3});
        vec32i_t *answer = vec32i_from({-2, 0, 1, 3});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({2, 0, -3, -6});
        vec32i_t *answer = vec32i_from({-6, -3, 0, 2});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({});
        vec32i_t *answer = vec32i_from({});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({0, 0, 0, 0});
        vec32i_t *answer = vec32i_from({0, 0, 0, 0});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({0, 0, 0, 0, 1, -1, 0, 0, 0});
        vec32i_t *answer = vec32i_from({-1, 0, 0, 0, 0, 0, 0, 0, 1});

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
    }
    {
        vec32i_t *vec = vec32i_from({
            56, 23, 19, 10, 6,  58, 21, 39, 50, 7,  45, 68, 46, 78, 98, 82, 14,
            83, 12, 63, 60, 79, 76, 75, 90, 65, 40, 77, 74, 94, 18, 2,  92, 73,
            13, 72, 61, 49, 52, 93, 9,  16, 8,  35, 88, 69, 97, 36, 99, 32,
        });
        vec32i_t *answer = vec32i_from({
            2,  6,  7,  8,  9,  10, 12, 13, 14, 16, 18, 19, 21, 23, 32, 35, 36,
            39, 40, 45, 46, 49, 50, 52, 56, 58, 60, 61, 63, 65, 68, 69, 72, 73,
            74, 75, 76, 77, 78, 79, 82, 83, 88, 90, 92, 93, 94, 97, 98, 99,
        });

        vec32i_bubble_sort(vec);
        if (!vec32i_eq(vec, answer)) {
            FAIL;
        }
        vec32i_free(vec);
        vec32i_free(answer);
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
    RUN_TEST(test_vec32i_bubble_sort);

    printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
    return 0;
}
