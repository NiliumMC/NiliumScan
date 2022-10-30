/*
 *  tui/include/tui/tui_button.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

struct tui_button {
    int y_pos;
    int x_pos;
    const char *name;
    const int name_str_len;
    const int element_id;
};

void print_buttons (const int, const int, const struct tui_button [], const int);

