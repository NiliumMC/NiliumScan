/*****************************************************************************
 *
 *    tui/src/tui_acts.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include "tui/tui_acts.h"

void print_act (const int y_pos, const int x, const int x_pos, const struct action *action_item) {
    int i, is_highlighted;

    if (x_pos > x - 3) return;
    mvaddch (y_pos, x_pos, ACS_LRCORNER);
    for (i = 0, is_highlighted = 0; action_item->name [i] && x_pos + i + 1 < x - 1; ++i) {
        if (action_item->name [i] == action_item->bind && !is_highlighted) {
            is_highlighted = 1;
            attron (A_BOLD | COLOR_PAIR (3));
            addch (action_item->name [i]);
            attroff (A_BOLD | COLOR_PAIR (3));
        } else {
            addch (action_item->name [i]);
        }
    } if (x_pos + i + 1 < x - 1) addch (ACS_LLCORNER);
}

