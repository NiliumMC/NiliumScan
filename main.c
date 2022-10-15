/*
 *  main.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdio.h>

#include "cli/cli.h"

int main (int argc, char **argv) {
    if (argc > 1)
        if (start_cli (argc, argv))
            return 0;

    return 0;
}

