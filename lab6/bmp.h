#pragma once

#include <stdio.h>

#include "image.h"

struct bmp_image;

struct bmp_image * bmp_image_read(FILE * file);
void bmp_image_free(struct bmp_image * file);

struct image * bmp_image_to_image(struct bmp_image * bmp_image);
