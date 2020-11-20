#include "mem.h"

#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>

#define HEAP_START ((void *) 0x04040000)
#define DEFAULT_HEAP_SIZE ((size_t) 4 * 1024)

#define BLOCK_MIN_SIZE ((size_t) 128 - sizeof(struct mem))

#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS 0x20
#endif

size_t heap_size = DEFAULT_HEAP_SIZE;

void * heap_mmap(void * addr, bool * strict) {
    void * heap = mmap(addr, heap_size, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

    if (strict) {
        *strict = (heap != MAP_FAILED);
    }

    if (heap != MAP_FAILED) {
        return heap;
    }

    return mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

struct mem mem_create(size_t capacity) {
    struct mem mem;

    mem.next = NULL;
    mem.capacity = capacity;
    mem.is_free = true;

    return mem;
}

void * heap_init(void * addr) {
    struct mem * heap = heap_mmap(addr, NULL);

    if (heap == MAP_FAILED) {
        return NULL;
    }

    *heap = mem_create(heap_size - sizeof(struct mem));
    return heap;
}

void heap_blocks_merge(struct mem * heap) {
    while (heap->next && heap->next->is_free) {
        if ((uint8_t *) heap->next == (uint8_t *) heap + sizeof(struct mem) + heap->capacity) {
            heap->capacity += sizeof(struct mem) + heap->next->capacity;
            heap->next = heap->next->next;
        }
    }
}

void * fetch_free_block(struct mem * heap) {
    while (!heap->is_free) {
        if (!heap->next) {
            if (!(heap->next = heap_init((uint8_t *) heap + sizeof(struct mem) + heap->capacity))) {
                return NULL;
            }
        }

        heap = heap->next;
    }

    heap_blocks_merge(heap);
    return heap;
}

void * malloc(size_t query) {
    static struct mem * heap = NULL;
    struct mem * current;
    struct mem * new;
    bool mmap_strict;

    if (!heap) {
        if (!(heap = heap_init(HEAP_START))) {
            return NULL;
        }
    }

    if (query < BLOCK_MIN_SIZE) {
        query = BLOCK_MIN_SIZE;
    }

    /* fetch first at least capacity block */

    current = heap;
    while ((current = fetch_free_block(current)) && current->capacity < query) {
        if (!current->next) {
            new = heap_mmap(((uint8_t *) current) + sizeof(struct mem) + current->capacity, &mmap_strict);

            if (new == MAP_FAILED) {
                return NULL;
            }

            if (mmap_strict) {
                current->capacity += heap_size;
            } else {
                *new = mem_create(heap_size - sizeof(struct mem));
                current->next = new;
                current = new;
            }
        } else {
            current = current->next;
        }
    }

    if (!current) {
        return NULL;
    }

    /* split if can */
    if (query + sizeof(struct mem) + BLOCK_MIN_SIZE <= current->capacity) {
        new = (struct mem *) (((uint8_t *) current) + sizeof(struct mem) + query);
        *new = mem_create(current->capacity - query - sizeof(struct mem));

        current->capacity = query;
        current->next = new;
    }

    current->is_free = false;
    return (uint8_t *) current + sizeof(struct mem);
}

void free(void * mem) {
    struct mem * heap;

    if (!mem) {
        return;
    }

    heap = (struct mem *) ((uint8_t *) mem - sizeof(struct mem));
    heap->is_free = true;

    heap_blocks_merge(heap);
}

bool set_page_size(size_t new_size) {
    struct rlimit max_size;

    if (getrlimit(RLIMIT_DATA, &max_size) != 0) {
        return false;
    }

    if (new_size < sizeof(struct mem) + BLOCK_MIN_SIZE || new_size > max_size.rlim_cur) {
        return false;
    }

    heap_size = new_size;
    return true;
}

void reset_page_size() {
    heap_size = DEFAULT_HEAP_SIZE;
}
