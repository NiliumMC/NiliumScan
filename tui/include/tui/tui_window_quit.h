/*
 *  tui/include/tui/tui_window_quit.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

/* If TUI Mode session continues true, otherwise false. */
extern bool is_open;

/* TMCScan quit dialog window. */
bool window_quit (const int ch, const int screen_height, const int screen_width,
        const char * const name, const MEVENT * const);

