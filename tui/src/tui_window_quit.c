/*
 *  tui/src/tui_window_quit.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_window_quit.h"
#include "tui/tui_utils.h"

#define WINDOW_HEIGHT 6
#define WINDOW_WIDTH 35

bool is_open = true;

bool window_quit (const int ch, const int y, const int x, const char *name) {
    int window_y_pos, window_x_pos;

    if (ch == 'q') {
        return false;
    } if (ch == 'y') {
        is_open = false;
        return false;
    }

    window_y_pos = (y >> 1) - (WINDOW_HEIGHT >> 1);
    window_x_pos = (x >> 1) - (WINDOW_WIDTH >> 1);

    print_clear_win_at (window_y_pos, window_x_pos, WINDOW_HEIGHT, WINDOW_WIDTH, name);
    refresh ();

    return true;
}

