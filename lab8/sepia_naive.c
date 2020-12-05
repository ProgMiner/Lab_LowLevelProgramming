#include <string.h>
#include <stdio.h>

#include <value.h>
#include <image.h>

static uint8_t sat(uint32_t a) {
    return a > 255 ? 255 : a;
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

    for (i = 0; i < length; ++i) {
        sepia_naive_one(image->pixels + i);
    }

    return NULL;
}
