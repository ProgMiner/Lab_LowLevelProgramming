#pragma once

#include <stddef.h>
#include <stdint.h>

struct pixel {
    uint8_t r, g, b;
    double x, y;
};

struct image {
    size_t pixels_count;
    struct pixel * pixels;
};

void image_print(struct image * image);
