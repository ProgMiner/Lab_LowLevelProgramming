#include "bmp.h"

#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

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

double min(double a, double b) {
    return a < b ? a : b;
}

void bmp_image_rotate(struct bmp_image * image, double angle) {
    double center_x, center_y, alpha,
           min_x = DBL_MAX,
           min_y = DBL_MAX,
           max_x = -DBL_MAX,
           max_y = -DBL_MAX;

    uint32_t x, y, i, j, k, base_x, base_y, pixels_count;

    struct {
        double x, y;
        struct bmp_pixel pixel;
    } * pixels;

    pixels_count = image->header.biWidth * image->header.biHeight;
    pixels = malloc(sizeof(*pixels) * pixels_count);

    center_x = ((double) image->header.biWidth) / 2;
    center_y = ((double) image->header.biHeight) / 2;

    for (y = 0, i = 0; y < image->header.biHeight; ++y) {
        for (x = 0; x < image->header.biWidth; ++x, ++i) {
            pixels[i].x = center_x + (x - center_x) * cos(angle) - (y - center_y) * sin(angle);
            pixels[i].y = center_y + (x - center_x) * sin(angle) + (y - center_y) * cos(angle);
            pixels[i].pixel = image->bitmap[i];

            if (min_x > pixels[i].x) {
                min_x = pixels[i].x;
            }

            if (min_y > pixels[i].y) {
                min_y = pixels[i].y;
            }

            if (max_x < pixels[i].x) {
                max_x = pixels[i].x;
            }

            if (max_y < pixels[i].y) {
                max_y = pixels[i].y;
            }
        }
    }

    image->header.biWidth = ceil(max_x - min_x + 1);
    image->header.biHeight = ceil(max_y - min_y + 1);
    free(image->bitmap);

    image->bitmap = calloc(image->header.biWidth * image->header.biHeight, sizeof(struct bmp_pixel));

    for (i = 0; i < pixels_count; ++i) {
        pixels[i].x -= min_x;
        pixels[i].y -= min_y;

        base_x = ceil(pixels[i].x);
        base_y = ceil(pixels[i].y);

        /* 0 - center
         * 1 - top
         * 2 - right
         * 3 - bottom
         * 4 - left
         */
        for (k = 0; k < 5; ++k) {
            switch (k) {
            case 0:
                x = base_x;
                y = base_y;
                alpha = (1 - abs(pixels[i].x - x)) * (1 - abs(pixels[i].y - y));
                break;

            case 1:
                x = base_x;
                y = base_y - 1;
                alpha = (1 - abs(x - pixels[i].x)) * (1 - min(pixels[i].y - y, 1));
                break;

            case 2:
                x = base_x + 1;
                y = base_y;
                alpha = (1 - min(x - pixels[i].x, 1)) * (1 - abs(y - pixels[i].y));
                break;

            case 3:
                x = base_x;
                y = base_y + 1;
                alpha = (1 - abs(x - pixels[i].x)) * (1 - min(y - pixels[i].y, 1));
                break;

            case 4:
                x = base_x - 1;
                y = base_y;
                alpha = (1 - min(pixels[i].x - x, 1)) * (1 - abs(y - pixels[i].y));
                break;
            }

            if (x >= 0 && x < image->header.biWidth
             && y >= 0 && y < image->header.biHeight) {
                j = y * image->header.biWidth + x;

                image->bitmap[j].r += (pixels[i].pixel.r - image->bitmap[j].r) * alpha;
                image->bitmap[j].g += (pixels[i].pixel.g - image->bitmap[j].g) * alpha;
                image->bitmap[j].b += (pixels[i].pixel.b - image->bitmap[j].b) * alpha;
            }
        }
    }

    free(pixels);
}

/* размытие */

bool bmp_image_print(const struct bmp_image * image, FILE * file) {
    uint32_t x, y, i;

    for (y = 0, i = 0; y < image->header.biHeight; ++y) {
        for (x = 0; x < image->header.biWidth; ++x, ++i) {
            if (fprintf(file, "\x1B[48;2;%d;%d;%dm  \x1B[0m",
                image->bitmap[i].r, image->bitmap[i].g, image->bitmap[i].b) < 1) {
                return false;
            }
        }

        if (putc('\n', file) == EOF) {
            return false;
        }
    }

    return true;
}

void bmp_image_repair_header(struct bmp_image * image) {

    /* Repair signature */
    image->header.bfType[0] = 'B';
    image->header.bfType[1] = 'M';

    /* Repair offset */
    image->header.bfOffBits = sizeof(struct bmp_header);

    /* Repair common parameters */
    image->header.biSize = 40;
    image->header.biPlanes = 1;
    image->header.biBitCount = 24;
    image->header.biCompression = 0;

    /* Calc size image */
    image->header.biSizeImage = image->header.biHeight * (image->header.biWidth * sizeof(struct bmp_pixel) + image->header.biWidth % 4);

    /* Calc file size */
    image->header.bfSize = image->header.bfOffBits + image->header.biSizeImage;
}

bool bmp_image_write(const struct bmp_image * image, FILE * file) {
    static uint8_t offsetBuffer[] = { 0, 0, 0 };
    int32_t row, rowOffset;

    if (fwrite(&(image->header), sizeof(struct bmp_header), 1, file) < 1) {
        return false;
    }

    rowOffset = image->header.biWidth % 4;
    for (row = image->header.biHeight - 1; row >= 0; --row) {
        if (fwrite(image->bitmap + row * image->header.biWidth,
            sizeof(struct bmp_pixel), image->header.biWidth, file) < image->header.biWidth) {
            return false;
        }

        if (fwrite(offsetBuffer, 1, rowOffset, file) < rowOffset) {
            return false;
        }
    }

    return true;
}
