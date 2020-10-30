#include "image.h"

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

size_t * image_get_pixels(struct image * image, size_t * width, size_t * height) {
    double min_x = DBL_MAX, max_x = -DBL_MAX, min_y = DBL_MAX, max_y = -DBL_MAX;
    uint32_t i, x, y;
    size_t * pixels;

    for (i = 0; i < image->pixels_count; ++i) {
        if (min_x > image->pixels[i].x) {
            min_x = image->pixels[i].x;
        }

        if (max_x < image->pixels[i].x) {
            max_x = image->pixels[i].x;
        }

        if (min_y > image->pixels[i].y) {
            min_y = image->pixels[i].y;
        }

        if (max_y < image->pixels[i].y) {
            max_y = image->pixels[i].y;
        }
    }

    *width = ceil(max_x - min_x) + 1;
    *height = ceil(max_y - min_y) + 1;

    pixels = malloc(sizeof(size_t) * *width * *height);
    for (i = 0; i < image->pixels_count; ++i) {
        x = (uint32_t) (image->pixels[i].x - min_x);
        y = (uint32_t) (image->pixels[i].y - min_x);
        pixels[x + y * *width] = i;
    }

    return pixels;
}

void image_print(struct image * image) {
    size_t width, height;
    uint32_t x, y, i;

    size_t * pixels = image_get_pixels(image, &width, &height);

    for (y = 0, i = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++i) {
            printf("\x1B[48;2;%d;%d;%dm  \x1B[0m",
                image->pixels[pixels[i]].r,
                image->pixels[pixels[i]].g,
                image->pixels[pixels[i]].b);
        }

        putchar('\n');
    }

    free(pixels);
}
