#include "mem_debug.h"

#include <stdint.h>

void memalloc_debug_struct_info(FILE * f, struct mem const * const address) {
    size_t i;

    fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n",
            (void *) address,
            address->capacity,
            address->is_free);

    for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i) {
        fprintf(f, "%X", (int) ((char *) address)[sizeof(struct mem) + i]);
    }

    putc('\n', f);
}

void memalloc_debug_heap(FILE * f, struct mem const * ptr) {
    for(; ptr; ptr = ptr->next) {
        memalloc_debug_struct_info(f, ptr);
    }
}

int main() {
    void * mem1 = malloc(4 * 1024 - sizeof(struct mem));

    uint32_t * mem;
    uint32_t ** mems;
    uint32_t i;

    set_page_size(8192);

    memalloc_debug_struct_info(stderr, (const struct mem *) ((uint8_t *) mem1 - sizeof(struct mem)));
    mem = malloc(sizeof(uint32_t));

    memalloc_debug_struct_info(stderr, (const struct mem *) ((uint8_t *) mem1 - sizeof(struct mem)));

    scanf("%u", mem);
    mems = malloc(sizeof(uint32_t *) * *mem);

    memalloc_debug_struct_info(stderr, (const struct mem *) ((uint8_t *) mem1 - sizeof(struct mem)));

    reset_page_size();

    for (i = 0; i < *mem; ++i) {
        mems[i] = malloc(sizeof(uint32_t));
        *(mems[i]) = i;

        printf("%u\n", *(mems[i]));
        memalloc_debug_heap(stderr, (const struct mem *) ((uint8_t *) mem - sizeof(struct mem)));
    }

    scanf("%*c");

    for (i = 0; i < *mem; ++i) {
        free(mems[i]);
    }

    free(mems);
    free(mem);

    memalloc_debug_heap(stderr, (const struct mem *) ((uint8_t *) mem - sizeof(struct mem)));
    return 0;
}
