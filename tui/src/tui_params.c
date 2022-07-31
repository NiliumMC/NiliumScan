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

void print_param (const int x_pos, const struct param *param_item) {
    int i, is_highlighted;

    mvaddch (0, x_pos, ACS_URCORNER);
    for (i = 0, is_highlighted = 0; param_item->name [i]; ++i) {
        if (param_item->name [i] == param_item->bind && !is_highlighted) {
            is_highlighted = 1;
            attron (A_BOLD | COLOR_PAIR (3));
            addch ((unsigned int) param_item->name [i]);
            attroff (A_BOLD | COLOR_PAIR (3));
        } else {
            addch ((unsigned int) param_item->name [i]);
        }
    } addch (ACS_ULCORNER);
}

int check_mouse_pos_params (const MEVENT *mouse_event, const struct param *params_list, const int num_params) {
    register int i, pos;

    for (i = 0, pos = 3; i < num_params; pos += params_list [i].len + 1, ++i)
        if (!mouse_event->y && mouse_event->x >= pos && mouse_event->x < pos + params_list [i].str_len)
            return i;

    return -1;
}

