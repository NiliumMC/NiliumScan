/*
 *  tui/include/tui/tui_main_actions.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

/* Main menu action element structure */
struct main_action {
    const char * const name;
    const int name_str_len;
    const char bind;
    bool (* const func) (const int, const int, const int, const char * const, const MEVENT * const);
    bool is_enabled;
};

/* Draw all available main menu
 * actions at the menu border.
 */
void print_main_actions (const int y_pos, const struct main_action actions_arr [], const int array_size);

/* Checks if the mouse click was
 * at some main menu action.
 * 
 * Returns array element id if found,
 * otherwise array_size + 1.
 */
int handle_mouse_on_main_actions (const MEVENT * const mouse_event, const int y_pos, const struct main_action actions_arr [], const int array_size);

