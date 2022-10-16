/*
 *  cli/src/cli.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdio.h>
#include <stdbool.h>

#include "cli/cli.h"
#include "cli/cli_parameters.h"

bool start_cli (int argc, char **argv) {
    handle_parameters (argc, argv);

    if (term_flags & tfb_use_cli) {

        return true;
    }

    return false;
}

