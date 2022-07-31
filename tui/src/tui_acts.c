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

void print_act (const int y_pos, const int x_pos, const struct action *action_item) {
    int i, is_highlighted;

    mvaddch (y_pos, x_pos, ACS_LRCORNER);
    for (i = 0, is_highlighted = 0; action_item->name [i]; ++i) {
        if (action_item->name [i] == action_item->bind && !is_highlighted) {
            is_highlighted = 1;
            attron (A_BOLD | COLOR_PAIR (3));
            addch ((unsigned int) action_item->name [i]);
            attroff (A_BOLD | COLOR_PAIR (3));
        } else {
            addch ((unsigned int) action_item->name [i]);
        }
    } addch (ACS_LLCORNER);
}

int check_mouse_pos_acts (const int y, const MEVENT *mouse_event, const struct action *actions_list, const int num_acts) {
    register int i, pos;

    for (i = 0, pos = 2; i < num_acts; pos += actions_list [i].len + 2, ++i)
        if (mouse_event->y == y && mouse_event->x >= pos && mouse_event->x < pos + actions_list [i].len)
            return i;

    return -1;
}

