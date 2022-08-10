/*****************************************************************************
 *
 *    cli/src/cli_parameters.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <unistd.h>
#include <getopt.h>

#define _GNU_SOURCE

#include "cli/cli_parameters.h"
#include "cli/cli_help.h"

struct option long_options [] = { { "help", no_argument, 0, 'h' } };

void parameter_handler (int argc, char **argv) {
    int arg, index = 0;

    while ((arg = getopt_long (argc, argv, "h", long_options, &index)) != -1) {
        switch (arg) {
            case 'h':
                print_help (0);
                break;

            default:
                print_help (1);
        }
    }
}

