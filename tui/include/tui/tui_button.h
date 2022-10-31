/*
 *  tui/include/tui/tui_button.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

struct tui_button {
    int y_pos;
    int x_pos;
    const char *name;
    const int name_str_len;
    const int element_id;
    const int up_element_id;
    const int down_element_id;
    const int left_element_id;
    const int right_element_id;
};

void print_buttons (const int, const int, const struct tui_button [], const int, const int);
void print_button (const int, const int, const struct tui_button *, const bool);

