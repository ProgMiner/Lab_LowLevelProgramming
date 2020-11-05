#pragma once

#include <stdbool.h>
#include <stddef.h>

struct args {
    const char * executable; /* Path to executable */
    const char * filename;   /* Path to input file */
    double       angle;      /* Rotation angle (degrees) */

    const char * output_filename; /* Path to output file (NULL == input file) */
    bool         output_stdout;   /* Output result to stdout instead of file */
    bool         print_ansi;      /* Do print result as ANSI escape codes instead of BMP */
    bool         print_help;      /* Do print help and quit */
} args;

bool parse_args(int argc, const char * argv[]);