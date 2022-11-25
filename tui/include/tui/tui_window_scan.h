/*
 *  tui/include/tui/tui_window_scan.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

/* TMCScan scan dialog window */
bool window_scan (const int ch, const int screen_height, const int screen_width,
        const char * const name, const MEVENT * const mouse_event);

