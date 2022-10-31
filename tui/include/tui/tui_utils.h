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

/* TODO: Change Direction Bindings Struct/Functions */

void print_clear_win_at (const int, const int, const int, const int, const char *);
void change_item (const enum item_change_direction, const struct tui_button [], const int);

