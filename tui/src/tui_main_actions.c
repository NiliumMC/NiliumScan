/*
 *  tui/src/tui_main_actions.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_main_actions.h"
#include "tui/tui_colors.h"

/* Draw one main menu action an the menu border. */
static void print_main_action (const struct main_action * const action);

void print_main_actions (const int y_pos, const struct main_action actions_arr [], const int array_size) {
    int counter;

    move (y_pos - 1, 1);
    for (counter = 0; counter < array_size; ++counter) {
        print_main_action (&actions_arr [counter]);
    }
}

int handle_mouse_on_main_actions (const MEVENT * const mouse_event, const int y_pos, const struct main_action actions_arr [], const int array_size) {
    register int counter, x_pos;

    if (y_pos - 1 == mouse_event->y) {
        for (counter = 0, x_pos = 1; counter < array_size; x_pos += actions_arr [counter].name_str_len + 2, ++counter) {
            if (mouse_event->x >= x_pos && mouse_event->x <= x_pos + actions_arr [counter].name_str_len + 1) {
                return counter;
            }
        }
    }

    return array_size + 1;
}

static void print_main_action (const struct main_action * const action) {
    bool is_highlighted;
    int counter;

    addch (ACS_LRCORNER);
    for (counter = 0, is_highlighted = 0; action->name [counter]; ++counter) {
        if (is_highlighted == false && action->name [counter] == action->bind) {
            is_highlighted = true;
            attron (A_BOLD | COLOR_PAIR (PAIR_GENERAL));
            addch ((unsigned int) action->name [counter]);
            standend ();
        } else {
            addch ((unsigned int) action->name [counter]);
        }
    } addch (ACS_LLCORNER);
}

