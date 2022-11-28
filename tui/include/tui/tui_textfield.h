/*
 *  tui/include/tui/tui_textfield.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#pragma once

#include <stdbool.h>

#include <ncurses.h>

/* Window textfield element structure. */
struct tui_textfield {
    int y_pos;
    int x_pos;
    const char * const name;
    const int name_str_len;
    const int textfield_len;
    const int buf_max_len;
    int buf_len;
    char *buf;
    const int element_id;
    const int up_element_id;
    const int down_element_id;
    const int left_element_id;
    const int right_element_id;
};

/* Draw the all textfields in the window. */
void print_textfields (const int window_y_pos, const int window_x_pos,
        const struct tui_textfield textfields_arr [],
        const int array_size, const int current_item_id);

/* Draw some textfield in the window. */
void print_textfield (const int window_y_pos, const int window_x_pos,
        const struct tui_textfield * const textfield,
        const bool is_highlighted);

/* Checks if the mouse click
 * was on the passed textfield element.
 *
 * Returns true if so,
 * otherwise false.
 */
extern bool is_mouse_click_on_textfield (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const struct tui_textfield * const textfield);

/* Searches the textfield element by ID.
 *
 * If found, returns the pointer
 * to the element, otherwise NULL.
 */
const struct tui_textfield * get_textfield_by_id (const int id,
        const struct tui_textfield textfields_arr [], const int array_size);

