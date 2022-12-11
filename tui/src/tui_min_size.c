/*
 *  tui/src/tui_min_size.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_min_size.h"
#include "tui/tui_colors.h"

bool is_quite_large = true;

void print_min_size (const int y, const int x) {
    int y_pos, x_pos;

    y_pos = (y >> 1) - 2;
    x_pos = (x >> 1) - 16;

    clear ();

    mvprintw (y_pos++, x_pos, "Your terminal is too small!");

    mvprintw (y_pos++, x_pos, "      Lines: %d/", MIN_SCR_LINES);
    attron (COLOR_PAIR ((y >= MIN_SCR_LINES) ? PAIR_POSITIVE : PAIR_NEGATIVE));
    printw ("%d", y);
    standend ();

    mvprintw (y_pos, x_pos, "      Cols: %d/", MIN_SCR_COLS);
    attron (COLOR_PAIR ((x >= MIN_SCR_COLS) ? PAIR_POSITIVE : PAIR_NEGATIVE));
    printw ("%d", x);
    standend ();

    refresh ();
}

