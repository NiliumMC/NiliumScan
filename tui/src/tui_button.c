/*
 *  tui/src/tui_button.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_button.h"
#include "tui/tui_colors.h"

void print_buttons (const int window_y_pos, const int window_x_pos,
        const struct tui_button buttons_arr [],
        const int array_size, const int current_item_id) {
    int counter;

    for (counter = 0; counter < array_size; ++counter) {
        if (buttons_arr [counter].element_id == current_item_id) {
            print_button (window_y_pos, window_x_pos, &buttons_arr [counter], true);
        } else {
            print_button (window_y_pos, window_x_pos, &buttons_arr [counter], false);
        }
    }
}

void print_button (const int window_y_pos, const int window_x_pos,
        const struct tui_button * const button,
        const bool is_highlighted) {
    if (is_highlighted) {
        attron (A_REVERSE | A_BOLD);
    }

    mvprintw (window_y_pos + button->y_pos, window_x_pos + button->x_pos, "<%s>", button->name);

    if (is_highlighted) {
        standend ();
    }
}

bool is_mouse_click_on_button (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const struct tui_button * const button) {
    if (mouse_event->y == window_y_pos + button->y_pos &&
            mouse_event->x >= window_x_pos + button->x_pos &&
            mouse_event->x <= window_x_pos + button->x_pos + button->name_str_len + 1) {
        return true;
    }

    return false;
}

const struct tui_button * get_button_by_id (const int id,
        const struct tui_button buttons_arr [], const int array_size) {
    int counter;

    for (counter = 0; counter < array_size; ++counter) {
        if (buttons_arr [counter].element_id == id) {
            return &buttons_arr [counter];
        }
    }

    return NULL;
}

