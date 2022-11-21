/*
 *  tui/include/tui/tui_main_actions.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
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

/* Draw all available main menu actions at the menu border. */
void print_main_actions (const int y_pos, const struct main_action actions_arr [], const int array_size);

