/*
 *  tui/src/tui_min_size.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_min_size.h"
#include "tui/tui_colors.h"

void print_min_size (const int y, const int x) {
    int y_pos, x_pos;

    y_pos = (y >> 1) - 2;
    x_pos = (x >> 1) - 16;

    clear ();

    mvprintw (y_pos++, x_pos, "Your terminal is too small!");

    mvprintw (y_pos++, x_pos, "      Lines: %d/", MIN_SCR_LINES);
    attron (COLOR_PAIR ((y >= MIN_SCR_LINES) ? pair_positive : pair_negative));
    printw ("%d", y);
    standend ();

    mvprintw (y_pos, x_pos, "      Cols: %d/", MIN_SCR_COLS);
    attron (COLOR_PAIR ((x >= MIN_SCR_COLS) ? pair_positive : pair_negative));
    printw ("%d", x);
    standend ();

    refresh ();
}

