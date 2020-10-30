#include "bmp.h"

#include <stdlib.h>

struct __attribute__((packed)) bmp_header {
    char     bfType[2];
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;

    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct bmp_pixel {
    uint8_t b, g, r;
};

struct bmp_image {
    struct bmp_header header;
    struct bmp_pixel * bitmap;
};

struct bmp_image * bmp_image_read(FILE * file) {
    struct bmp_image * image = malloc(sizeof(struct bmp_image));
    int32_t row, rowOffset;

    size_t read_count = fread(&(image->header), sizeof(struct bmp_header), 1, file);
    if (read_count < 1) {
        free(image);
        return NULL;
    }

    if ((image->header.bfType[0] != 'B' || image->header.bfType[1] != 'M')              /* Check file type signature */
     || (image->header.bfSize != image->header.bfOffBits + image->header.biSizeImage)   /* Check size */
     || (image->header.biPlanes != 1)                                                   /* Check biPlanes */
     || (image->header.biBitCount != 24)                                                /* Check pixel bits count, only 24 is supported */
     || (image->header.biCompression != 0)                                              /* Check biCompression, only 0 is supported */
    ) {
        free(image);
        return NULL;
    }

    /* Check file size */
    if (fseek(file, 0L, SEEK_END) || ftell(file) != image->header.bfSize) {
        free(image);
        return NULL;
    }

    /* Go to bitmap */
    if (fseek(file, image->header.bfOffBits, SEEK_SET)) {
        free(image);
        return NULL;
    }

    image->bitmap = malloc(sizeof(struct bmp_pixel) * image->header.biWidth * image->header.biHeight);

    rowOffset = image->header.biWidth % 4;
    for (row = image->header.biHeight - 1; row >= 0; --row) {
        read_count = fread(image->bitmap + row * image->header.biWidth, sizeof(struct bmp_pixel), image->header.biWidth, file);

        if (read_count < image->header.biWidth) {
            bmp_image_free(image);
            return NULL;
        }

        if (fseek(file, rowOffset, SEEK_CUR)) {
            bmp_image_free(image);
            return NULL;
        }
    }

    return image;
}

struct image * bmp_image_to_image(struct bmp_image * bmp_image) {
    struct image * image;
    uint64_t x, y, i;

    if (bmp_image == NULL) {
        return NULL;
    }

    image = malloc(sizeof(struct image));
    image->pixels_count = bmp_image->header.biWidth * bmp_image->header.biHeight;
    image->pixels = malloc(sizeof(struct pixel) * image->pixels_count);

    for (y = 0, i = 0; y < bmp_image->header.biHeight; ++y) {
        for (x = 0; x < bmp_image->header.biWidth; ++x, ++i) {
            image->pixels[i].r = bmp_image->bitmap[i].r;
            image->pixels[i].g = bmp_image->bitmap[i].g;
            image->pixels[i].b = bmp_image->bitmap[i].b;
            image->pixels[i].x = x;
            image->pixels[i].y = y;
        }
    }

    return image;
}

void bmp_image_free(struct bmp_image * image) {
    if (image) {
        free(image->bitmap);
        free(image);
    }
}
