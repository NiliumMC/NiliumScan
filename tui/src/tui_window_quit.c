/*
 *  tui/src/tui_window_quit.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_window_quit.h"

bool is_open = true;

bool window_quit (const int ch, const int y, const int x, const char *name) {
    if (ch == 'q') {
        return false;
    } if (ch == 'y') {
        is_open = false;
        return false;
    }

    mvprintw (y / 2, x / 2, "%s", name);
    refresh ();

    return true;
}

