/*
 *  tui/src/tui_window_scan.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include "tui/tui_window_scan.h"
#include "tui/tui_textfield.h"
#include "tui/tui_colors.h"
#include "tui/tui_utils.h"

#define WINDOW_HEIGHT 7
#define WINDOW_WIDTH 36

#define BUTTONS_COUNT 2
#define TEXTFIELDS_COUNT 1

/* Window element that selected by default */
#define DEFAULT_ITEM_ID 1

static bool start_button_action (void);
static bool cancel_button_action (void);

static int current_item_id = DEFAULT_ITEM_ID;

/* Initialization of buttons array */
static const struct tui_button buttons_arr [BUTTONS_COUNT] = {
    { 4, 8,  "Start",  5, start_button_action,  0, 2, 2, 1, 1 },
    { 4, 20, "Cancel", 6, cancel_button_action, 1, 2, 2, 0, 0 }
};

static const struct tui_textfield textfields_arr [TEXTFIELDS_COUNT] = {
    { 2, 2, "IP", 2, 17, 25, 14, "127.0.0.1pumba", 2, 0, 0, 0, 0 }
};

bool window_scan (const int ch, const int screen_height, const int screen_width,
        const char * const name, const MEVENT * const mouse_event) {
    static int window_y_pos, window_x_pos;
    enum item_change_direction direction;
    enum item_type type, tmp_item_type;
    int tmp_item_id;

    type = get_item_type_by_id (current_item_id, buttons_arr, BUTTONS_COUNT,
            textfields_arr, TEXTFIELDS_COUNT);

    /* If mouse button is pressed outside the window,
     * then window will be closed.
     *
     * Same behavior with q key.
     */
    if (ch == 'q') {
        return false;
    } else if (ch == KEY_MOUSE) {
        if (is_mouse_click_out_window (mouse_event, window_y_pos, window_x_pos,
                WINDOW_HEIGHT, WINDOW_WIDTH)) {
            return false;
        } else if (is_mouse_left_click (mouse_event)) {
            if (is_mouse_click_on_item (mouse_event, window_y_pos, window_x_pos,
                        &tmp_item_id, buttons_arr, BUTTONS_COUNT)) {
                if (current_item_id != tmp_item_id) {
                    change_item (window_y_pos, window_x_pos, current_item_id, tmp_item_id,
                            buttons_arr, BUTTONS_COUNT, textfields_arr, TEXTFIELDS_COUNT);
                    current_item_id = tmp_item_id;
                } else {
                    tmp_item_type = type;
                    if (tmp_item_type == BUTTON_TYPE) {
                        if (get_button_by_id (tmp_item_id, buttons_arr, BUTTONS_COUNT)->func () == false) {
                            return false;
                        }
                    }
                }
            }
        } else if (is_mouse_left_double_click (mouse_event)) {
            if (is_mouse_click_on_item (mouse_event, window_y_pos, window_x_pos,
                        &tmp_item_id, buttons_arr, BUTTONS_COUNT)) {
                if (current_item_id != tmp_item_id) {
                    change_item (window_y_pos, window_x_pos, current_item_id, tmp_item_id,
                            buttons_arr, BUTTONS_COUNT, textfields_arr, TEXTFIELDS_COUNT);
                    current_item_id = tmp_item_id;
                    tmp_item_type = get_item_type_by_id (tmp_item_id, buttons_arr, BUTTONS_COUNT,
                            textfields_arr, TEXTFIELDS_COUNT);
                } else {
                    tmp_item_type = type;
                }

                if (tmp_item_type == BUTTON_TYPE) {
                    if (get_button_by_id (tmp_item_id, buttons_arr, BUTTONS_COUNT)->func () == false) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    if (check_bind (ch, type, binds_arr, 4, &direction) == true) {
        current_item_id = directional_change_item (window_y_pos, window_x_pos,
                current_item_id, direction, buttons_arr, BUTTONS_COUNT,
                textfields_arr, TEXTFIELDS_COUNT);
        return true; /* Skip other key checks when item is changed. */
    }

    if (is_enter_key (ch)) {
        if (type == BUTTON_TYPE) {
            /* If the button is pressend and it's
             * function returns false then window is closed.
             */
            if (get_button_by_id (current_item_id, buttons_arr, BUTTONS_COUNT)->func () == false) {
                current_item_id = DEFAULT_ITEM_ID;
                return false;
            }
        }

        return true;
    }

    /* Redraw the window if it's have some updates. */
    if (ch == KEY_RESIZE || ch == OK) {
        window_y_pos = (screen_height >> 1) - (WINDOW_HEIGHT >> 1);
        window_x_pos = (screen_width >> 1) - (WINDOW_WIDTH >> 1);

        print_clear_win_at (window_y_pos, window_x_pos, WINDOW_HEIGHT, WINDOW_WIDTH, name);
        print_buttons (window_y_pos, window_x_pos, buttons_arr, BUTTONS_COUNT, current_item_id);
        print_textfields (window_y_pos, window_x_pos, textfields_arr, TEXTFIELDS_COUNT, current_item_id);
        refresh ();
    }

    return true;
}

static bool start_button_action (void) {
    return true;
}

static bool cancel_button_action (void) {
    return false;
}

