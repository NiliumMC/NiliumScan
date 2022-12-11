/*
 *  tui/src/tui_textfield.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include "tui/tui_textfield.h"
#include "tui/tui_colors.h"

void print_textfields (const int window_y_pos, const int window_x_pos,
        const struct tui_textfield textfields_arr [],
        const int array_size, const int current_item_id) {
    register int counter;

    for (counter = 0; counter < array_size; ++counter) {
        if (textfields_arr [counter].element_id == current_item_id) {
            print_textfield (window_y_pos, window_x_pos, &textfields_arr [counter], true);
        } else {
            print_textfield (window_y_pos, window_x_pos, &textfields_arr [counter], false);
        }
    }
}

void print_textfield (const int window_y_pos, const int window_x_pos,
        const struct tui_textfield * const textfield,
        const bool is_highlighted) {
    mvprintw (window_y_pos + textfield->y_pos, window_x_pos + textfield->x_pos, "%s ", textfield->name);

    if (is_highlighted) {
        attron (COLOR_PAIR (PAIR_GENERAL) | A_REVERSE | A_BOLD);
    } else {
        attron (COLOR_PAIR (PAIR_GENERAL) | A_REVERSE);
    }

    if (textfield->textfield_len > textfield->buf_len) {
        printw ("%*s%s%*s", (textfield->textfield_len - textfield->buf_len) / 2, "", textfield->buf,
                (textfield->textfield_len - textfield->buf_len + 1) / 2, "");
        if (is_highlighted) {
            standend ();
            attron (A_REVERSE | A_BLINK);
            mvaddch (window_y_pos + textfield->y_pos,
                    window_x_pos + textfield->x_pos + textfield->name_str_len + 1
                    + textfield->buf_len + ((textfield->textfield_len - textfield->buf_len) / 2), ACS_BOARD);
        }
    } else {
        if (is_highlighted) {
            printw ("%s", textfield->buf + (textfield->buf_len - textfield->textfield_len + 1));
            standend ();
            attron (A_REVERSE | A_BLINK);
            addch (ACS_BOARD);
        } else {
            printw ("%s", textfield->buf + (textfield->buf_len - textfield->textfield_len));
        }
    }

    standend ();
}

bool is_mouse_click_on_textfield (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const struct tui_textfield * const textfield) {
    if (mouse_event->y == window_y_pos + textfield->y_pos &&
            mouse_event->x >= window_x_pos + textfield->x_pos &&
            mouse_event->x <= window_x_pos + textfield->x_pos +
            textfield->name_str_len + textfield->textfield_len) {
        return true;
    }

    return false;
}

const struct tui_textfield * get_textfield_by_id (const int id,
        const struct tui_textfield textfields_arr [], const int array_size) {
    int counter;

    for (counter = 0; counter < array_size; ++counter) {
        if (textfields_arr [counter].element_id == id) {
            return &textfields_arr [counter];
        }
    }

    return NULL;
}

