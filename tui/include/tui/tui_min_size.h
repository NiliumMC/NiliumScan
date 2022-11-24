/*
 *  tui/include/tui/tui_min_size.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#define MIN_SCR_LINES 24
#define MIN_SCR_COLS 80

/* If the terminal window size is larger
 * or equal than 80x24 true,
 * otherwise false.
 */
extern bool is_quite_large;

/* Prints warning if terminal window is so small. */
void print_min_size (const int y, const int x);

