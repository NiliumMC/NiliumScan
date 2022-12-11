/*
 *  tui/include/tui/tui_button.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

/* Window button element structure. */
struct tui_button {
    int y_pos;
    int x_pos;
    const char * const name;
    const int name_str_len;
    bool (* const func) (void);
    const int element_id;
    const int up_element_id;
    const int down_element_id;
    const int left_element_id;
    const int right_element_id;
};

/* Draw the all buttons in the window. */
void print_buttons (const int window_y_pos, const int window_x_pos,
        const struct tui_button buttons_arr [],
        const int array_size, const int current_item_id);

/* Draw some button in the window. */
void print_button (const int window_y_pos, const int window_x_pos,
        const struct tui_button * const button,
        const bool is_highlighted);

/* Checks if the mouse click
 * was on the passed button element.
 *
 * Returns true if so,
 * otherwise false.
 */
bool is_mouse_click_on_button (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const struct tui_button * const button);

/* Searches the button element by ID.
 *
 * If found, returns the pointer
 * to the element, otherwise NULL.
 */
const struct tui_button * get_button_by_id (const int id,
        const struct tui_button buttons_arr [], const int array_size);

