#include <stdio.h>

#include "linked_list.h"

int main() {
    list_t * list = NULL;
    int value;

    while (scanf("%d", &value) == 1) {
        list_add_front(value, &list);
    }

    printf("Sum: %ld\n", list_sum(list));
    list_free(list);
    return 0;
}
