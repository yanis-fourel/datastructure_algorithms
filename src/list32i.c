#include <stdint.h>
#include <stdlib.h>

#include "alloc.h"
#include "list32i.h"

// segv if we go out of bounds
// Could traverse from the tail if idx > size/2
static node32i_t *_get_node(node32i_t *head, size_t idx) {
    for (size_t i = 0; i < idx; ++i) {
        head = head->next; // Kids, really, don't do linked list
    }
    return head;
}

list32i_t *list32i_new() {
    return (list32i_t *)calloc(1, sizeof(list32i_t));
}

void list32i_free(list32i_t *list) {
    while (list->length) {
        list32i_pop_back(list);
    }
    free(list);
}

void list32i_push_back(list32i_t *list, int32_t val) {
    node32i_t *node = calloc_or_panic(1, sizeof(node32i_t));
    node->data = val;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    ++list->length;
}

void list32i_push_front(list32i_t *list, int32_t val) {
    node32i_t *node = calloc_or_panic(1, sizeof(node32i_t));
    node->data = val;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    ++list->length;
}

void list32i_push_idx(list32i_t *list, size_t idx, int32_t val) {
    if (idx == 0) {
        list32i_push_front(list, val);
        return;
    }
    while (list->length < idx) {
        list32i_push_back(list, 0);
    }

    node32i_t *node = calloc_or_panic(1, sizeof(node32i_t));
    node->data = val;
    node32i_t *prevnode = _get_node(list->head, idx - 1);
    node->next = prevnode->next;
    node->prev = prevnode;
    prevnode->next = node;
    ++list->length;
}

int32_t list32i_pop_back(list32i_t *list) {
    return list32i_pop_idx(list, list->length - 1);
}

int32_t list32i_pop_front(list32i_t *list) {
    return list32i_pop_idx(list, 0);
}

int32_t list32i_pop_idx(list32i_t *list, size_t idx) {
    if (list->length <= idx) {
        return 0;
    }

    node32i_t *node = _get_node(list->head, idx);
    int32_t res = node->data;
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    free(node);
    --list->length;
    return res;
}

int32_t list32i_get_idx(list32i_t *list, size_t idx) {
    if (list->length <= idx) {
        return 0;
    }

    return _get_node(list->head, idx)->data;
}

void list32i_set_idx(list32i_t *list, size_t idx, int32_t val) {
    if (list->length >= idx) {
        _get_node(list->head, idx)->data = val;
        return;
    }
    while (list->length < idx - 1) {
        list32i_push_back(list, 0);
    }
    list32i_push_back(list, val);
}
