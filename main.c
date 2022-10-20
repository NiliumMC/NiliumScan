/*
 *  main.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdio.h>

#include "cli/cli.h"
#include "tui/tui.h"

int main (int argc, char **argv) {
    if (argc > 1) {
        if (start_cli (argc, argv)) {
            return 0;
        }
    }

    if (start_tui ()) {
        show_menu ();
        end_tui ();
        return 0;
    } else {
        end_tui ();
        fputs ("Curses not initialized normally!\n", stderr);
        return 1;
    }
}

