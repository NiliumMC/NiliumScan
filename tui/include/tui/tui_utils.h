/*
 *  tui/include/tui/tui_utils.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

#include "tui/tui_button.h"

enum item_change_direction {
    icd_up,
    icd_down,
    icd_left,
    icd_right
};

enum item_type {
    button_type,
    textfield_type /* TODO */
};

struct item_change_direction_binding {
    const int bind;
    const bool is_for_buttons;
    const bool is_for_textfields;
};

struct item_change_direction_bindings {
    const enum item_change_direction direction;
    const int array_size;
    const struct item_change_direction_binding binds_arr [];
};

void print_clear_win_at (const int, const int, const int, const int, const char *);
void change_item (const enum item_change_direction, const struct tui_button [], const int);
bool check_bind (const int, const enum item_type, const struct item_change_direction_bindings *, const int);

