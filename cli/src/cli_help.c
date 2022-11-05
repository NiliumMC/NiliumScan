/*
 *  cli/src/cli_help.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdlib.h>
#include <stdio.h>

#include "cli/cli_help.h"

void print_help (const int exit_code) {
    puts ("TMCScan - " TMCS_DESCRIPTION "\n"
          "GitHub: https://github.com/0Magenta0/TMCScan\n"
          "Released Under GPLv3 | Copyright (C) 2022 _Magenta_\n\n"

          "Usage: tmcscan [args]\n\n"

          "  -h --help\t\t\tPrint this message\n"
          "     --cli\t\t\tEnable CLI Mode\n\n"

          "Version: " TMCS_VERSION "\n"
          "Author: _Magenta_\n");
    exit (exit_code);
}

