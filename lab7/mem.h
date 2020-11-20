#pragma once

#include <stdbool.h>
#include <stddef.h>

#pragma pack(push, 1)
struct mem {
    struct mem * next;
    size_t capacity;
    bool is_free;
};
#pragma pack(pop)

void * malloc(size_t query);
void free(void * mem);
