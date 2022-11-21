/*
 *  tui/include/tui/tui_window_quit.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

extern bool is_open;

bool window_quit (const int, const int, const int, const char *, const MEVENT * const);

