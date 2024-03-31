///
/// A simple double-linked list. This is just for education purposes, in any
/// real life situation, please please use a vec32i instead.
/// See https://youtu.be/YQs6IC-vgmo
///
/// Seriously, don't use linked lists
///

#pragma once

#include <stdint.h>
#include <sys/types.h>

typedef struct s_list32i {
    // First node
    struct s_node32i *head;
    // Last node
    struct s_node32i *tail;
    size_t length;
} list32i_t;

typedef struct s_node32i {
    int32_t data;
    struct s_node32i *prev;
    struct s_node32i *next;
} node32i_t;

list32i_t *list32i_new();
void list32i_free(list32i_t *list);

void list32i_push_back(list32i_t *list, int32_t val);
void list32i_push_front(list32i_t *list, int32_t val);

/// If we push an out of bound index, it will grow the list with zeros until it
/// can put the value at wanted index
void list32i_push_idx(list32i_t *list, size_t idx, int32_t val);

/// Removes the last element and returns it.
/// For empty list, returns 0.
int32_t list32i_pop_back(list32i_t *list);

/// Removes the first element and returns it.
/// For empty list, returns 0.
int32_t list32i_pop_front(list32i_t *list);

/// Removes the element at given index and returns it.
/// If the index does not exist, returns 0.
int32_t list32i_pop_idx(list32i_t *list, size_t idx);

/// Just use a vec already
/// If the index does not exist, returns 0.
int32_t list32i_get_idx(list32i_t *list, size_t idx);

/// If the index is out of bound, it will grow the list with zeros until it can
/// set the value at wanted index
void list32i_set_idx(list32i_t *list, size_t idx, int32_t val);
