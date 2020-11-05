#include "args.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

struct args args = {
    NULL,
    NULL,
    .0,
    NULL,
    false,
    false,
    false,
    false,
    false,
    false
};

bool parse_args(int argc, const char * argv[]) {
    bool delimiterArrived = false;
    uint32_t currentArgument = 0;
    const char * currentArg;
    uint32_t i;

    args.executable = argv[0];

    for (i = 1; i < argc; ++i) {
        currentArg = argv[i];

        if (!delimiterArrived && argv[i][0] == '-') {
            if (currentArg[1] != '-') {
                while (*(++currentArg)) {
                    switch (*currentArg) {
                    case 'o':
                        args.output_filename = argv[++i];
                        break;

                    case 'O':
                        args.output_stdout = true;
                        break;

                    case 'a':
                        args.print_ansi = true;
                        break;

                    case 'h':
                        args.print_help = true;
                        return true;

                    case 'b':
                        args.do_blur = true;
                        break;

                    case 'd':
                        args.do_blur = true;
                        args.do_dilate = true;
                        break;

                    case 'e':
                        args.do_blur = true;
                        args.do_erode = true;
                        break;

                    default:
                        fprintf(stderr, "Warning: skipped unknown option -%c.\n", *currentArg);
                    }
                }

                continue;
            }

            ++currentArg;
            ++currentArg;

            if (*currentArg == 0) {
                delimiterArrived = true;
                continue;
            }

            if (strcmp("output", currentArg) == 0) {
                args.output_filename = argv[++i];
            } else if (strcmp("stdout", currentArg) == 0) {
                args.output_stdout = true;
            } else if (strcmp("ascii", currentArg) == 0) {
                args.print_ansi = true;
            } else if (strcmp("help", currentArg) == 0) {
                args.print_help = true;
                return true;
            } else if (strcmp("blur", currentArg) == 0) {
                args.do_blur = true;
            } else if (strcmp("dilate", currentArg) == 0) {
                args.do_blur = true;
                args.do_dilate = true;
            } else if (strcmp("erode", currentArg) == 0) {
                args.do_blur = true;
                args.do_erode = true;
            } else {
                fprintf(stderr, "Warning: skipped unknown option --%s.\n", currentArg);
            }

            continue;
        }

        switch (++currentArgument) {
        case 1:
            args.filename = currentArg;
            break;

        case 2:
            if (sscanf(argv[i], "%lf", &args.angle) != 1) {
                fprintf(stderr, "Error: angle value must be number, \"%s\" passed instead.\n", currentArg);
                return false;
            }

            break;

        default:
            fprintf(stderr, "Warning: unused argument passed \"%s\".\n", currentArg);
        }
    }

    if (!args.print_help && currentArgument < 2) {
        fprintf(stderr, "Error: not all required arguments passed!\n");
        return false;
    }

    return true;
}
