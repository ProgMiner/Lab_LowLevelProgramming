#include <stdbool.h>
#include <stdio.h>

#include "linked_list.h"

struct sum_min_max {
    long sum;
    int min;
    int max;
};

void print_int_spaced(int a) {
    printf("%d ", a);
}

void print_int_lined(int a) {
    printf("%d\n", a);
}

int square_int(int a) {
    return a * a;
}

int cube_int(int a) {
    return a * a * a;
}

int abs_int(int a) {
    if (a < 0) {
        return -a;
    }

    return a;
}

bool save(list_t * lst, const char * filename) {
    FILE * f = fopen(filename, "wb");

    if (f == NULL) {
        return false;
    }

    for (; lst; lst = list_node_at(lst, 1)) {
        if (fprintf(f, "%d ", list_get(lst, 0)) <= 0) {
            return false;
        }
    }

    return !fclose(f);
}

bool load(list_t ** lst, const char * filename) {
    FILE * f = fopen(filename, "rb");
    int value;

    if (f == NULL) {
        return false;
    }

    while (fscanf(f, "%d", &value) != 1) {
        list_add_back(value, lst);
    }

    if (ferror(f)) {
        return false;
    }

    return !fclose(f);
}

void * update_sum_min_max(int a, void * fold_data) {
    struct sum_min_max * smm = fold_data;

    smm->sum += a;

    if (smm->min > a) {
        smm->min = a;
    }

    if (smm->max < a) {
        smm->max = a;
    }

    return smm;
}

int dup_int(int a) {
    return 2 * a;
}

int main() {
    struct sum_min_max smm = { 0, 2147483647, -2147483648 };
    list_t * list = NULL;
    list_t * squares;
    list_t * cubes;
    list_t * bits;
    int value;

    while (scanf("%d", &value) == 1) {
        list_add_back(value, &list);
    }

    list_foreach(list, print_int_spaced);
    putchar('\n');

    list_foreach(list, print_int_lined);

    squares = list_map(square_int, list);
    list_foreach(squares, print_int_spaced);
    list_free(squares);
    putchar('\n');

    cubes = list_map(cube_int, list);
    list_foreach(cubes, print_int_spaced);
    list_free(cubes);
    putchar('\n');

    list_foldl(&smm, update_sum_min_max, list);
    printf("Sum: %ld. Min: %d. Max: %d.\n", smm.sum, smm.min, smm.max);

    list_map_mut(abs_int, list);
    list_foreach(list, print_int_spaced);
    putchar('\n');

    bits = list_iterate(1, 10, dup_int);
    list_foreach(bits, print_int_spaced);
    list_free(bits);
    putchar('\n');

    list_free(list);
    return 0;
}
