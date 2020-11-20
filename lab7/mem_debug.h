#pragma once

#include <stdio.h>

#include "mem.h"

#define DEBUG_FIRST_BYTES 4

void memalloc_debug_struct_info(FILE * f, struct mem const * const address);
void memalloc_debug_heap(FILE * f, struct mem const * ptr);
