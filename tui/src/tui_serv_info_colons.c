/*
 *  tui/src/tui_serv_info_colons.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_serv_info_colons.h"
#include "tui/tui_colors.h"

/* Draw some server column header at the menu border */
static void print_serv_info_colon (const int x_pos, const struct serv_info_colon * const info_colon);

void print_serv_info_colons (const struct serv_info_colon colons_arr [], const int array_size) {
    register int counter, x_pos;

    for (counter = 0, x_pos = 2; counter < array_size; x_pos += colons_arr [counter].field_len + 1, ++counter) {
        print_serv_info_colon (x_pos, &colons_arr [counter]);
    }
}

static void print_serv_info_colon (const int x_pos, const struct serv_info_colon * const info_colon) {
    bool is_highlighted;
    register int counter;

    mvaddch (0, x_pos, ACS_URCORNER);
    for (counter = 0, is_highlighted = 0; info_colon->name [counter]; ++counter) {
        if (is_highlighted == false && info_colon->name [counter] == info_colon->bind) {
            is_highlighted = true;
            attron (A_BOLD | COLOR_PAIR (PAIR_GENERAL));
            addch ((unsigned int) info_colon->name [counter]);
            standend ();
        } else {
            addch ((unsigned int) info_colon->name [counter]);
        }
    } addch (ACS_ULCORNER);
}

