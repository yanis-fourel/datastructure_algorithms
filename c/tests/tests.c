#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "_bitvec.h"
#include "list32i.h"
#include "vec.h"

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

int test_vec() {
    i32vec_t *myvec = i32vec_new(0, 0);

    if (myvec->size != 0) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }

    {
        i32vec_t *tmp = i32vec_from({});
        if (!i32vec_eq(myvec, tmp)) {
            FAIL;
        }
        i32vec_free(tmp);
    }

    for (int i = 0; i < 10; i++) {
        i32vec_append(myvec, i);
    }

    i32vec_remove(myvec, 7);
    {
        i32vec_t *tmp = i32vec_from({0, 1, 2, 3, 4, 5, 6, 8, 9});
        if (!i32vec_eq(myvec, tmp)) {
            i32vec_print(myvec);
            i32vec_print(tmp);
            FAIL;
        }
        i32vec_free(tmp);
    }

    i32vec_remove(myvec, 8);
    {
        i32vec_t *tmp = i32vec_from({0, 1, 2, 3, 4, 5, 6, 8});
        if (!i32vec_eq(myvec, tmp)) {
            FAIL;
        }
        i32vec_free(tmp);
    }

    i32vec_remove(myvec, 0);
    {
        i32vec_t *tmp = i32vec_from({1, 2, 3, 4, 5, 6, 8});
        if (!i32vec_eq(myvec, tmp)) {
            FAIL;
        }
        i32vec_free(tmp);
    }

    for (int i = 0; i < 6; i++) {
        i32vec_remove(myvec, 1);
    }
    {
        i32vec_t *tmp = i32vec_from({1});
        if (!i32vec_eq(myvec, tmp)) {
            FAIL;
        }
        i32vec_free(tmp);
    }

    i32vec_remove(myvec, 0);
    {
        i32vec_t *tmp = i32vec_from({});
        if (!i32vec_eq(myvec, tmp)) {
            FAIL;
        }
        i32vec_free(tmp);
    }

    i32vec_free(myvec);

    return 0;
}

int test_i32vec_search() {
    i32vec_t *vec = i32vec_from({2, 1, -1, 9, -6, 0, 3});

    if (i32vec_search(vec, 2) != 0) {
        FAIL;
    }
    if (i32vec_search(vec, 1) != 1) {
        FAIL;
    }
    if (i32vec_search(vec, -1) != 2) {
        FAIL;
    }
    if (i32vec_search(vec, 9) != 3) {
        FAIL;
    }
    if (i32vec_search(vec, -6) != 4) {
        FAIL;
    }
    if (i32vec_search(vec, 0) != 5) {
        FAIL;
    }
    if (i32vec_search(vec, 3) != 6) {
        FAIL;
    }
    if (i32vec_search(vec, 4) != -1) {
        FAIL;
    }
    if (i32vec_search(vec, -2) != -1) {
        FAIL;
    }

    i32vec_free(vec);
    return 0;
}

int cmp_int32(const void *ptra, const void *ptrb) {
    return *((int32_t *)ptra) - *((int32_t *)ptrb);
}

int test_i32vec_binary_search() {
    i32vec_t *vec = i32vec_from({-32, -6, 0, 1, 3, 7, 32, 1024});

    if (i32vec_search_binary(vec, -32, cmp_int32) != 0) {
        FAIL;
    }
    if (i32vec_search_binary(vec, -6, cmp_int32) != 1) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 0, cmp_int32) != 2) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 1, cmp_int32) != 3) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 3, cmp_int32) != 4) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 7, cmp_int32) != 5) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 32, cmp_int32) != 6) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 1024, cmp_int32) != 7) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 89, cmp_int32) != -1) {
        FAIL;
    }
    if (i32vec_search_binary(vec, -1, cmp_int32) != -1) {
        FAIL;
    }
    if (i32vec_search_binary(vec, 9000, cmp_int32) != -1) {
        FAIL;
    }
    if (i32vec_search_binary(vec, -9000, cmp_int32) != -1) {
        FAIL;
    }

    i32vec_free(vec);
    return 0;
}

int test_i32vec_bubble_sort() {
    {
        i32vec_t *vec = i32vec_from({1, 0, 3});
        i32vec_t *answer = i32vec_from({0, 1, 3});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            i32vec_print(vec);
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({-2, 0, 1, 3});
        i32vec_t *answer = i32vec_from({-2, 0, 1, 3});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({2, 0, -3, -6});
        i32vec_t *answer = i32vec_from({-6, -3, 0, 2});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({});
        i32vec_t *answer = i32vec_from({});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({0, 0, 0, 0});
        i32vec_t *answer = i32vec_from({0, 0, 0, 0});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({0, 0, 0, 0, 1, -1, 0, 0, 0});
        i32vec_t *answer = i32vec_from({-1, 0, 0, 0, 0, 0, 0, 0, 1});

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }
    {
        i32vec_t *vec = i32vec_from({
            56, 23, 19, 10, 6,  58, 21, 39, 50, 7,  45, 68, 46, 78, 98, 82, 14,
            83, 12, 63, 60, 79, 76, 75, 90, 65, 40, 77, 74, 94, 18, 2,  92, 73,
            13, 72, 61, 49, 52, 93, 9,  16, 8,  35, 88, 69, 97, 36, 99, 32,
        });
        i32vec_t *answer = i32vec_from({
            2,  6,  7,  8,  9,  10, 12, 13, 14, 16, 18, 19, 21, 23, 32, 35, 36,
            39, 40, 45, 46, 49, 50, 52, 56, 58, 60, 61, 63, 65, 68, 69, 72, 73,
            74, 75, 76, 77, 78, 79, 82, 83, 88, 90, 92, 93, 94, 97, 98, 99,
        });

        i32vec_bubble_sort(vec, cmp_int32);
        if (!i32vec_eq(vec, answer)) {
            i32vec_print(vec);
            FAIL;
        }
        i32vec_free(vec);
        i32vec_free(answer);
    }

    return 0;
}

int test_bitvec() {
    bitvec_t *myvec = bitvec_new(0, 0);

    if (myvec->size != 0) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }

    {
        bitvec_t *tmp = bitvec_from({});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    for (int i = 0; i < 20; i++) {
        bitvec_append(myvec, i % 2);
    }
    if (myvec->size != 20) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }

    bitvec_remove(myvec, 7);
    if (myvec->size != 19) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from(
            {false, true, false, true, false, true, false, false, true, false,
             true, false, true, false, true, false, true, false, true});
        if (!bitvec_eq(myvec, tmp)) {
            bitvec_print(myvec);
            bitvec_print(tmp);
            FAIL;
        }
        bitvec_free(tmp);
    }

    if (bitvec_get(myvec, 0) != false) {
        FAIL;
    }
    if (bitvec_get(myvec, 1) != true) {
        FAIL;
    }
    if (bitvec_get(myvec, 2) != false) {
        FAIL;
    }
    if (bitvec_get(myvec, 3) != true) {
        FAIL;
    }
    if (bitvec_get(myvec, 7) != false) {
        FAIL;
    }
    if (bitvec_get(myvec, 8) != true) {
        FAIL;
    }
    if (bitvec_get(myvec, 9) != false) {
        FAIL;
    }
    if (bitvec_get(myvec, 10) != true) {
        FAIL;
    }
    if (bitvec_get(myvec, 15) != false) {
        FAIL;
    }
    if (bitvec_get(myvec, 16) != true) {
        FAIL;
    }
    if (bitvec_get(myvec, 17) != false) {
        FAIL;
    }

    bitvec_remove(myvec, 14);
    if (myvec->size != 18) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from({false, true, false, true, false, true,
                                     false, false, true, false, true, false,
                                     true, false, false, true, false, true});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    bitvec_set(myvec, 0, true);
    bitvec_set(myvec, 1, false);
    bitvec_set(myvec, 2, false);
    bitvec_set(myvec, 7, true);
    bitvec_set(myvec, 8, false);
    bitvec_set(myvec, 9, true);
    bitvec_set(myvec, 15, false);
    bitvec_set(myvec, 16, true);
    bitvec_set(myvec, 17, false);
    {
        bitvec_t *tmp = bitvec_from({true, false, false, true, false, true,
                                     false, true, false, true, true, false,
                                     true, false, false, false, true, false});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    bitvec_remove(myvec, 0);
    if (myvec->size != 17) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from({false, false, true, false, true, false,
                                     true, false, true, true, false, true,
                                     false, false, false, true, false});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    bitvec_set(myvec, 33, true);
    if (myvec->size != 34) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from(
            {false, false, true,  false, true,  false, true,  false, true,
             true,  false, true,  false, false, false, true,  false, false,
             false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, true});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    for (int i = 0; i < 33; i++) {
        bitvec_remove(myvec, 1);
    }
    if (myvec->size != 1) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from({false});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    bitvec_remove(myvec, 0);
    if (myvec->size != 0) {
        printf("size is %zu\n", myvec->size);
        FAIL;
    }
    {
        bitvec_t *tmp = bitvec_from({});
        if (!bitvec_eq(myvec, tmp)) {
            FAIL;
        }
        bitvec_free(tmp);
    }

    bitvec_free(myvec);

    return 0;
}

bitvec_t *create_two_crystal_balls_input(size_t size, ssize_t answer) {
    bitvec_t *vec = bitvec_new(size, size);

    if (answer >= 0) {
        for (; (size_t)answer < size; ++answer) {
            bitvec_set(vec, answer, true);
        }
    }
    return vec;
}
int test_bitvec_two_crystal_balls() {
    {
        ssize_t answer = 234;
        bitvec_t *vec = create_two_crystal_balls_input(1000, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = 0;
        bitvec_t *vec = create_two_crystal_balls_input(1000, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = 999;
        bitvec_t *vec = create_two_crystal_balls_input(1000, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = -1;
        bitvec_t *vec = create_two_crystal_balls_input(1000, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = -1;
        bitvec_t *vec = create_two_crystal_balls_input(0, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = 0;
        bitvec_t *vec = create_two_crystal_balls_input(1, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    {
        ssize_t answer = 5;
        bitvec_t *vec = create_two_crystal_balls_input(6, answer);
        if (bitvec_two_crystal_balls(vec) != answer) {
            FAIL;
        }
        bitvec_free(vec);
    }
    return 0;
}

// -------------------------------------------

int test_list32i_push_back() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_back(list, 42);
    if (list->length != 1) {
        FAIL;
    }
    val = list32i_get_idx(list, 0);
    if (val != 42) {
        FAIL;
    }

    list32i_push_back(list, 84);
    if (list->length != 2) {
        FAIL;
    }
    val = list32i_get_idx(list, 1);
    if (val != 84) {
        FAIL;
    }

    list32i_free(list);
    return 0;
}

int test_list32i_push_front() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_front(list, 42);
    if (list->length != 1) {
        FAIL;
    }
    val = list32i_get_idx(list, 0);
    if (val != 42) {
        FAIL;
    }

    list32i_push_front(list, 84);
    if (list->length != 2) {
        FAIL;
    }
    val = list32i_get_idx(list, 0);
    if (val != 84) {
        FAIL;
    }

    list32i_free(list);
    return 0;
}

int test_list32i_push_idx() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_idx(list, 0, 42);
    if (list->length != 1) {
        FAIL;
    }
    val = list32i_get_idx(list, 0);
    if (val != 42) {
        FAIL;
    }

    list32i_push_idx(list, 0, 84);
    if (list->length != 2) {
        FAIL;
    }
    val = list32i_get_idx(list, 0);
    if (val != 84) {
        FAIL;
    }

    list32i_push_idx(list, 5, 168);
    if (list->length != 6) {
        FAIL;
    }
    val = list32i_get_idx(list, 2);
    if (val != 0) {
        FAIL;
    }
    val = list32i_get_idx(list, 3);
    if (val != 0) {
        FAIL;
    }
    val = list32i_get_idx(list, 4);
    if (val != 0) {
        FAIL;
    }
    val = list32i_get_idx(list, 5);
    if (val != 168) {
        FAIL;
    }

    list32i_free(list);
    return 0;
}

int test_list32i_pop_back() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_back(list, 42);
    list32i_push_back(list, 84);
    list32i_push_back(list, 168);
    list32i_push_back(list, 336);

    if (list->length != 4) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 3) {
        FAIL;
    }
    if (val != 336) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 2) {
        FAIL;
    }
    if (val != 168) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 1) {
        FAIL;
    }
    if (val != 84) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 42) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    val = list32i_pop_back(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    list32i_free(list);
    return 0;
}

int test_list32i_pop_front() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_back(list, 42);
    list32i_push_back(list, 84);
    list32i_push_back(list, 168);
    list32i_push_back(list, 336);

    if (list->length != 4) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 3) {
        FAIL;
    }
    if (val != 42) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 2) {
        FAIL;
    }
    if (val != 84) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 1) {
        FAIL;
    }
    if (val != 168) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 336) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    val = list32i_pop_front(list);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    list32i_free(list);
    return 0;
}

int test_list32i_pop_idx() {
    list32i_t *list = list32i_new();
    int32_t val;

    if (list->length != 0) {
        FAIL;
    }

    list32i_push_back(list, 42);
    list32i_push_back(list, 84);
    list32i_push_back(list, 168);
    list32i_push_back(list, 336);

    if (list->length != 4) {
        FAIL;
    }

    val = list32i_pop_idx(list, 1);
    if (list->length != 3) {
        FAIL;
    }
    if (val != 84) {
        FAIL;
    }

    val = list32i_pop_idx(list, 2);
    if (list->length != 2) {
        FAIL;
    }
    if (val != 336) {
        FAIL;
    }

    val = list32i_pop_idx(list, 5);
    if (list->length != 2) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    val = list32i_pop_idx(list, 0);
    if (list->length != 1) {
        FAIL;
    }
    if (val != 42) {
        FAIL;
    }

    val = list32i_pop_idx(list, 0);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 168) {
        FAIL;
    }

    val = list32i_pop_idx(list, 0);
    if (list->length != 0) {
        FAIL;
    }
    if (val != 0) {
        FAIL;
    }

    list32i_free(list);
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

    RUN_TEST(test_vec);
    RUN_TEST(test_i32vec_search);
    RUN_TEST(test_i32vec_binary_search);
    RUN_TEST(test_i32vec_bubble_sort);
    RUN_TEST(test_bitvec);
    RUN_TEST(test_bitvec_two_crystal_balls);

    RUN_TEST(test_list32i_push_back);
    RUN_TEST(test_list32i_push_front);
    RUN_TEST(test_list32i_push_idx);
    RUN_TEST(test_list32i_pop_back);
    RUN_TEST(test_list32i_pop_front);
    RUN_TEST(test_list32i_pop_idx);

    printf("--------------\nOK: %d\nKO: %d\n", ok, ko);
    return 0;
}
