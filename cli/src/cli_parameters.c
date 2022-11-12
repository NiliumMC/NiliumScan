/*
 *  cli/src/cli_parameters.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <unistd.h>
#include <getopt.h>

#define _GNU_SOURCE

#include "cli/cli_parameters.h"
#include "cli/cli_help.h"

enum term_flags_list {
    TFL_GET_HELP = 0x68, /* h */
    TFL_USE_CLI = 0x01
};

enum term_flags_bitset term_flags;

struct option long_flags [] = {
    { "help", no_argument, 0, TFL_GET_HELP },
    { "cli",  no_argument, 0, TFL_USE_CLI }
};

void handle_parameters (const int argc, char ** const argv) {
    int arg, opt_index = 0;

    while ((arg = getopt_long (argc, argv, "h", long_flags, &opt_index)) != -1) {
        switch ((enum term_flags_list) arg) {
            case TFL_GET_HELP: {
                print_help (0);
                break;
            }

            case TFL_USE_CLI: {
                term_flags |= TFB_USE_CLI;
                break;
            }

            default: print_help (1);
        }
    }
}

