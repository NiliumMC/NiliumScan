/*
 *  tui/src/tui.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include <stdbool.h>
#include <locale.h>

#include "tui/tui.h"

bool start_tui (void) {
    setlocale (LC_ALL, "");

    initscr ();
    if (keypad (stdscr, true) == ERR) return false;
    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    nodelay (stdscr, true);
    curs_set (0);
    noecho ();
    raw ();

    if (has_colors ()) {
        start_color ();
    } else {
        endwin ();
        return false;
    }

    if (can_change_color ()) {
        assume_default_colors (231, 16);
    } else {

    }

    endwin ();
    return true;
}

