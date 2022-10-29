/*
 *  tui/include/tui/tui_main_actions.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

struct main_action {
    const char *name;
    const int name_str_len;
    const char bind;
    bool (*const func) (const int, const int, const int, const char *);
    bool is_enabled;
};

void print_main_actions (const int, const struct main_action [], const int);
int check_opened_windows (const struct main_action [], const int);

