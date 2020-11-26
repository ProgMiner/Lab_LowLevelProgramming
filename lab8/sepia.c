#include <stdio.h>

#include <value.h>
#include <image.h>

void do_sepia(const uint32_t r[12], const uint32_t g[12], const uint32_t b[12], uint32_t result[12]);

static uint8_t sat(uint32_t a) {
    return a > 255 ? 255 : a;
}

static void process_cluster(struct pixel * cluster, uint8_t size) {
    static uint32_t r[12], g[12], b[12], result[12];
    uint8_t i, j;

    for (i = 0; i < size; ++i) {
        for (j = 0; j < 3; ++j) {
            r[i * 3 + j] = cluster[i].red;
            g[i * 3 + j] = cluster[i].green;
            b[i * 3 + j] = cluster[i].blue;
        }
    }

    do_sepia(r, g, b, result);
    for (i = 0, j = 0; i < size; ++i) {
        cluster[i].red   = sat(result[j++]);
        cluster[i].green = sat(result[j++]);
        cluster[i].blue  = sat(result[j++]);
    }
}

const char * sepia(struct image * image, uint32_t argc, const struct value * argv) {
    const uint32_t length = image->width * image->height;
    const uint32_t clusters = length / 4;
    uint32_t i;

    for (i = 0; i < clusters; ++i) {
        process_cluster(image->pixels + i * 4, 4);
    }

    if (length % 4 > 0) {
        process_cluster(image->pixels + i * 4, length % 4);
    }

    return NULL;
}


static void sepia_naive_one(struct pixel * const pixel) {
    static const float c[3][3] = {
        { .393f, .769f, .189f },
        { .349f, .686f, .168f },
        { .272f, .543f, .131f }
    };

    struct pixel const old = * pixel;

    pixel->red   = sat(old.red * c[0][0] + old.green * c[0][1] + old.blue * c[0][2]);
    pixel->green = sat(old.red * c[1][0] + old.green * c[1][1] + old.blue * c[1][2]);
    pixel->blue  = sat(old.red * c[2][0] + old.green * c[2][1] + old.blue * c[2][2]);
}

const char * sepia_naive(struct image * image, uint32_t argc, const struct value * argv) {
    const uint32_t length = image->width * image->height;
    uint32_t i;

    for (i = 0; i < length; i++) {
        sepia_naive_one(image->pixels + i);
    }

    return NULL;
}
