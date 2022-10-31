/*
 *  tui/include/tui/tui_min_size.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

#define MIN_SCR_LINES 24
#define MIN_SCR_COLS 80

extern bool is_quite_large;

void print_min_size (const int, const int);
