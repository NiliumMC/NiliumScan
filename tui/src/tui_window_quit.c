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

static bool yes_button_action (void);
static bool no_button_action (void);

bool is_open = true;

static int current_item_id = DEFAULT_ITEM_ID;

static const struct tui_button buttons_arr [BUTTONS_COUNT] = {
    { 3, 11, "Yes", 3, yes_button_action, 0, 0, 0, 1, 1 },
    { 3, 21, "No",  2, no_button_action,  1, 1, 1, 0, 0 }
};

bool window_quit (const int ch, const int screen_height, const int screen_width, const char * const name) {
    static int window_y_pos, window_x_pos;
    enum item_change_direction direction;
    enum item_type type;

    type = get_item_type_by_id (current_item_id, buttons_arr, BUTTONS_COUNT);
    if (check_bind (ch, type, binds_arr, 4, &direction) == true) {
        current_item_id = change_item (window_y_pos, window_x_pos, current_item_id, direction, buttons_arr, BUTTONS_COUNT);
        return true; /* Skip Other Key Checks When Item Is Changed */
    }

    if (is_enter_key (ch)) {
        if (type == button_type) {
            if (get_button_by_id (current_item_id, buttons_arr, BUTTONS_COUNT)->func () == false) {
                current_item_id = DEFAULT_ITEM_ID;
                return false;
            }
        }
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

static bool yes_button_action (void) {
    is_open = false;
    return false;
}

static bool no_button_action (void) {
    return false;
}

