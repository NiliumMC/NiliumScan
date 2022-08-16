/*****************************************************************************
 *
 *    tui/src/tui_quit.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include "tui/tui_utils.h"
#include "tui/tui_quit.h"

#define BOX_HEIGHT 5 /* Height Of Quit Window */
#define BOX_WIDTH 21 /* Width Of Quit Window */

#define BUTTONS_COUNT 2 /* Count Of Quit Window's Buttons */

int is_open = 1;
int quit_current = 1; /* Selected Now Quit TUI Element */

/* Quit Window's Buttons List */
struct tui_button quit_buttons [BUTTONS_COUNT] = { { 0, 0, COLOR_PAIR (2), "Yes", 3, 0, 0, 0, 1, 1 },
                                                   { 0, 0, COLOR_PAIR (1), "No", 2, 1, 1, 1, 0, 0 } };

int act_quit (const MEVENT *mouse_event, const int ch, const int y, const int x, const char *name) {
    static int y_pos, x_pos;
    int tmp_current;

    if (ch == OK) {
        quit_current = 1;
    } if (ch == 'q') {
        return 0;
    } if (ch == 'y') {
        quit_current = 0;
    } if (ch == 'n') {
        quit_current = 1;
    }

    if (check_move_key (ch)) {
        find_next_item (ch, 0, &quit_current, quit_buttons, BUTTONS_COUNT, 0, 0);
    }

    if (check_enter_key (ch)) {
        if (quit_current == 0) {
            is_open = 0;
            return 0;
        } else {
            return 0;
        }
    }

    if (ch == KEY_MOUSE) {
        if (check_mouse_click (mouse_event)) {
            tmp_current = find_next_item (ch, mouse_event, &quit_current, quit_buttons, BUTTONS_COUNT, 0, 0);

            if (quit_current == tmp_current && tmp_current == 0) {
                is_open = 0;
                return 0;
            } if (quit_current == tmp_current && tmp_current == 1) {
                return 0;
            }
        } if (check_mouse_double_click (mouse_event)) {
            tmp_current = find_next_item (ch, mouse_event, &quit_current, quit_buttons, BUTTONS_COUNT, 0, 0);

            if (tmp_current >= 0) {
                if (quit_current == 0) {
                    is_open = 0;
                    return 0;
                } if (quit_current == 1) {
                    return 0;
                }
            }
        } if (!check_mouse_pos_box (mouse_event->y, mouse_event->x, y_pos, x_pos, BOX_HEIGHT, BOX_WIDTH)) {
                return 0;
        }
    }

    y_pos = (y >> 1) - (BOX_HEIGHT >> 1);
    x_pos = (x >> 1) - (BOX_WIDTH >> 1);
    quit_buttons [0].y_pos = y_pos + 4;
    quit_buttons [0].x_pos = x_pos + 2;
    quit_buttons [1].y_pos = y_pos + 4;
    quit_buttons [1].x_pos = x_pos + 8;

    print_clear_win_at (y_pos, x_pos, BOX_HEIGHT, BOX_WIDTH, name);
    mvaddstr (y_pos + 2, x_pos + 2, "Are you sure?");
    print_tui_buttons (quit_current, quit_buttons, BUTTONS_COUNT);

    refresh ();
    return 1;
}

