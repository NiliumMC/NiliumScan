/*
 *  cli/src/cli.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>
#include <stdio.h>

#include "cli/cli_parameters.h"
#include "cli/cli.h"

bool start_cli (const int argc, char ** const argv) {
    handle_parameters (argc, argv);

    if (term_flags & TFB_USE_CLI) {

        return true;
    }

    return false;
}

