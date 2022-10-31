/*
 *  tui/src/tui_window_quit.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_window_quit.h"
#include "tui/tui_utils.h"

#define WINDOW_HEIGHT 6
#define WINDOW_WIDTH 36

#define BUTTONS_COUNT 2

#define DEFAULT_ITEM_ID 1

bool is_open = true;

static int current_item_id = DEFAULT_ITEM_ID;

static const struct tui_button buttons_arr [BUTTONS_COUNT] = {
    { 3, 11, "Yes", 3, 0, 0, 0, 1, 1 },
    { 3, 21, "No", 2, 1, 1, 1, 0, 0 }
};

bool window_quit (const int ch, const int screen_height, const int screen_width, const char *name) {
    static int window_y_pos, window_x_pos;

    if (ch == 'q') {
        current_item_id = DEFAULT_ITEM_ID;
        return false;
    } if (ch == 'y') {
        is_open = false;
        return false;
    }

    if (ch == KEY_RESIZE || ch == OK) {
        window_y_pos = (screen_height >> 1) - (WINDOW_HEIGHT >> 1);
        window_x_pos = (screen_width >> 1) - (WINDOW_WIDTH >> 1);

        print_clear_win_at (window_y_pos, window_x_pos, WINDOW_HEIGHT, WINDOW_WIDTH, name);
        mvprintw (window_y_pos + 2, window_x_pos + 6, "Are you sure about that?");
        print_buttons (window_y_pos, window_x_pos, buttons_arr, BUTTONS_COUNT, current_item_id);
        refresh ();
    }

    return true;
}

