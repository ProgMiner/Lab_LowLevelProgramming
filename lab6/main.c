#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"

#define RETCOD_OK    0
#define RETCOD_FOPEN 1
#define RETCOD_FCLOS 2
#define RETCOD_BADIM 3

int main() {
    struct bmp_image * bmp_image;
    FILE * file = fopen("test.bmp", "rb");
    struct image * image;

    if (!file) {
        return RETCOD_FOPEN;
    }

    bmp_image = bmp_image_read(file);
    if (fclose(file)) {
        return RETCOD_FCLOS;
    }

    image = bmp_image_to_image(bmp_image);
    if (!image) {
        return RETCOD_BADIM;
    }

    bmp_image_free(bmp_image);

    image_print(image);

    free(image);
    return RETCOD_OK;
}
