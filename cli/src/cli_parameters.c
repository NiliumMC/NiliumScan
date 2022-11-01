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
    tfl_get_help = 0x68, /* h */
    tfl_use_cli = 0x01
};

enum term_flags_bitset term_flags;

struct option long_flags [] = {
    { "help", no_argument, 0, tfl_get_help },
    { "cli",  no_argument, 0, tfl_use_cli }
};

void handle_parameters (int argc, char **argv) {
    int arg, opt_index = 0;

    while ((arg = getopt_long (argc, argv, "h", long_flags, &opt_index)) != -1) {
        switch ((enum term_flags_list) arg) {
            case tfl_get_help:
                print_help (0);
                break;

            case tfl_use_cli:
                term_flags |= tfb_use_cli;
                break;

            default:
                print_help (1);
        }
    }
}

