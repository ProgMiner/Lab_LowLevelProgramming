#include "bmp.h"

#include <stdint.h>
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

enum bmp_image_read_result
bmp_image_read(struct bmp_image ** bmp_image, FILE * file) {
    struct bmp_image * image = malloc(sizeof(struct bmp_image));
    int32_t row, rowOffset;

    size_t read_count = fread(&(image->header), sizeof(struct bmp_header), 1, file);
    if (read_count < 1) {
        free(image);
        return BMPREAD_BADFILE;
    }

    /* Check file type signature */
    if (image->header.bfType[0] != 'B' || image->header.bfType[1] != 'M') {
        free(image);
        return BMPREAD_BADSIGN;
    }

    if ((image->header.biSizeImage         /* Check size if biSizeImage != 0 */
     && (image->header.bfSize != image->header.bfOffBits + image->header.biSizeImage))
     || (image->header.biPlanes != 1)      /* Check biPlanes */
     || (image->header.biBitCount != 24)   /* Check pixel bits count, only 24 is supported */
     || (image->header.biCompression != 0) /* Check biCompression, only 0 is supported */
    ) {
        free(image);
        return BMPREAD_BADMETA;
    }

    /* Check file size */
    if (fseek(file, 0L, SEEK_END)) {
        free(image);
        return BMPREAD_BADFILE;
    }

    if (ftell(file) != image->header.bfSize) {
        free(image);
        return BMPREAD_BADMETA;
    }

    /* Go to bitmap */
    if (fseek(file, image->header.bfOffBits, SEEK_SET)) {
        free(image);
        return BMPREAD_BADFILE;
    }

    image->bitmap = malloc(sizeof(struct bmp_pixel) * image->header.biWidth * image->header.biHeight);

    rowOffset = image->header.biWidth % 4;
    for (row = image->header.biHeight - 1; row >= 0; --row) {
        read_count = fread(image->bitmap + row * image->header.biWidth, sizeof(struct bmp_pixel), image->header.biWidth, file);

        if (read_count < image->header.biWidth) {
            bmp_image_free(image);
            return BMPREAD_BADFILE;
        }

        if (fseek(file, rowOffset, SEEK_CUR)) {
            bmp_image_free(image);
            return BMPREAD_BADFILE;
        }
    }

    *bmp_image = image;
    return BMPREAD_OK;
}

void bmp_image_free(struct bmp_image * image) {
    if (image) {
        free(image->bitmap);
        free(image);
    }
}

void bmp_image_print(const struct bmp_image * image, FILE * file) {
    uint32_t x, y, i;

    for (y = 0, i = 0; y < image->header.biHeight; ++y) {
        for (x = 0; x < image->header.biWidth; ++x, ++i) {
            fprintf(file, "\x1B[48;2;%d;%d;%dm  \x1B[0m",
                image->bitmap[i].r,
                image->bitmap[i].g,
                image->bitmap[i].b);
        }

        putc('\n', file);
    }
}

/* поворачивание, размытие, сохранение */
