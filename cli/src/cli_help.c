/*****************************************************************************
 *
 *    cli/src/cli_help.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "cli/cli_help.h"

void print_help (int exit_code) {
    puts ("TMCScan - " TMCS_DESCRIPTION "\n"
          "GitHub: https://github.com/0Magenta0/TMCScan\n"
          "Released Under GPLv3 | Copyright (C) 2022 _Magenta_\n\n"

          "Usage: tmcscan [args]\n\n"

          "  -h --help\t\tPrint this message\n"
          "  -x --force-no-xterm-colors\t\tPrint this message\n\n"

          "Version: " TMCS_VERSION "\n"
          "Author: _Magenta_\n");
    exit (exit_code);
}

