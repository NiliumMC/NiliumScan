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

