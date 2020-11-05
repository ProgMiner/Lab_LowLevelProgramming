#pragma once

#include <stdbool.h>
#include <stdio.h>

struct bmp_image;

enum bmp_image_read_result {
    BMPREAD_BADFILE,
    BMPREAD_BADSIGN,
    BMPREAD_BADMETA,
    BMPREAD_OK
};

enum bmp_image_read_result
bmp_image_read(struct bmp_image ** bmp_image, FILE * file);

void bmp_image_free(struct bmp_image * file);

bool bmp_image_print(const struct bmp_image * bmp_image, FILE * file);

void bmp_image_repair_header(struct bmp_image * bmp_image);
bool bmp_image_write(const struct bmp_image * bmp_image, FILE * file);
