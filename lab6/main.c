#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include "bmp.h"
#include "args.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define RETCOD_OK   0
#define RETCOD_ARGP 1
#define RETCOD_FILE 2
#define RETCOD_BMPR 3

#define HELP_MSG (\
    "Usage: %s [options] <filename> <angle>\n"\
    "  - filename - path to input file\n"\
    "  - angle - rotation angle (in degrees, real)\n"\
    "  - options - some of following options:\n"\
    "    - -o/--output <filename> - path to output file (default equal to input file)\n"\
    "    - -O/--stdout - output to stdout instead of any file\n"\
    "    - -a/--ascii - output as ANSI escape codes instead of BMP\n"\
    "    - -h/--help - print this help message and exit\n"\
)

int8_t yesno() {
    int c = getchar(), nc;

    while (c != '\n' && c != EOF) {
        nc = getchar();

        switch (nc) {
        case '\n':
        case EOF:
            break;

        default:
            continue;
        }

        break;
    }

    switch (c) {
    case 'y':
        return 1;

    case 'n':
    case EOF:
        return 0;

    default:
        return -1;
    }
}

int main(int argc, const char * argv[]) {
    struct bmp_image * image;
    FILE * file;

    if (!parse_args(argc, argv) && !args.print_help) {
        fprintf(stderr, HELP_MSG, args.executable);
        return RETCOD_ARGP;
    }

    if (args.print_help) {
        printf(HELP_MSG, args.executable);
        return RETCOD_OK;
    }

    if (!(file = fopen(args.filename, "rb"))) {
        perror("Error");
        return RETCOD_FILE;
    }

    switch (bmp_image_read(&image, file)) {
    case BMPREAD_OK:
        break;

    case BMPREAD_BADFILE:
        fputs("Error: bad BMP file.\n", stderr);
        return RETCOD_FILE;

    default:
        fputs("Error: bad BMP file contents.\n", stderr);
        return RETCOD_BMPR;
    }

    if (fclose(file)) {
        perror("Error");
        return RETCOD_FILE;
    }

    /* bmp_image_print(image, stdout); */
    bmp_image_rotate(image, args.angle * M_PI / 180);

    if (args.do_blur) {
        /* bmp_image_print(image, stdout); */

        if (args.do_dilate) {
            bmp_image_dilate(image);
        } else if (args.do_erode) {
            bmp_image_erode(image);
        } else {
            bmp_image_blur(image);
        }
    }

    if (args.output_stdout) {
        file = stdout;
    } else if (args.output_filename) {
        while (!access(args.output_filename, F_OK)) {
            printf("File %s is already exists, do you want to overwrite it? y/n: ", args.output_filename);

            switch (yesno()) {
            case -1:
                continue;

            case 0:
                return RETCOD_FILE;
            }

            break;
        }

        if (!(file = fopen(args.output_filename, "wb"))) {
            perror("Error");
            return RETCOD_FILE;
        }
    } else {
        while (args.print_ansi) {
            fputs("Are you sure you want to overwrite the BMP file with ANSI? y/n: ", stdout);

            switch (yesno()) {
            case -1:
                continue;

            case 0:
                return RETCOD_FILE;
            }

            break;
        }

        if (!(file = fopen(args.filename, "wb"))) {
            perror("Error");
            return RETCOD_FILE;
        }
    }

    if (args.print_ansi) {
        if (!bmp_image_print(image, file)) {
            perror("Error");
            return RETCOD_FILE;
        }
    } else {
        bmp_image_repair_header(image);

        if (!bmp_image_write(image, file)) {
            perror("Error");
            return RETCOD_FILE;
        }
    }

    if (file != stdout && fclose(file)) {
        perror("Error");
        return RETCOD_FILE;
    }

    return RETCOD_OK;
}
