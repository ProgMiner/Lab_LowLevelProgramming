#include "linked_list.h"

#include <stdlib.h>

#define __list_new (malloc(sizeof(list_t)))
#define __list_foreach(_lst) for (; _lst; _lst = _lst->next)

struct list {
    int value;
    list_t * next;
};

list_t * list_create(int value) {
    list_t * list = __list_new;

    list->value = value;
    list->next = NULL;
    return list;
}

list_t * list_node_at(list_t * list, unsigned int index) {
    while (index-- > 0) {
        if (list == NULL) {
            return NULL;
        }

        list = list->next;
    }

    return list;
}

void list_free(list_t * list) {
    list_t * prev = list;

    while ((list = list->next)) {
        free(prev);

        prev = list;
    }

    free(prev);
}

unsigned int list_length(const list_t * list) {
    unsigned int length = 0;

    __list_foreach (list) {
        ++length;
    }

    return length;
}

int list_get(const list_t * list, unsigned int index) {
    const list_t * node = list_node_at((list_t *) list, index);

    if (node == NULL) {
        return 0;
    }

    return node->value;
}

long list_sum(const list_t * list) {
    long sum = 0;

    __list_foreach (list) {
        sum += list->value;
    }

    return sum;
}

void list_add_front(int value, list_t ** list) {
    list_t * new_list;

    if (!list) {
        return;
    }

    new_list = __list_new;
    new_list->value = value;
    new_list->next = *list;
    *list = new_list;
}

void list_add_back(int value, list_t ** list) {
    list_t * last = NULL;
    list_t * current;

    if (!list) {
        return;
    }

    current = *list;
    __list_foreach (current) {
        last = current;
    }

    if (!last) {
        list_add_front(value, list);
        return;
    }

    last->next = list_create(value);
}
