/*****************************************************************************
 *
 *    tui/src/tui_params.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include "tui/tui_params.h"

void print_param (const int x, const int x_pos, const struct param *param_item) {
    int i, is_highlighted;

    if (x_pos > x - 3) return;
    mvaddch (0, x_pos, ACS_URCORNER);
    for (i = 0, is_highlighted = 0; param_item->name [i] && x_pos + i + 1 < x - 1; ++i) {
        if (param_item->name [i] == param_item->bind && !is_highlighted) {
            is_highlighted = 1;
            attron (A_BOLD | COLOR_PAIR (3));
            addch (param_item->name [i]);
            attroff (A_BOLD | COLOR_PAIR (3));
        } else {
            addch (param_item->name [i]);
        }
    } if (x_pos + i + 1 < x - 1) addch (ACS_ULCORNER);
}

