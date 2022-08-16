/*****************************************************************************
 *
 *    tui/src/tui_about.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include "tui/tui_about.h"
#include "tui/tui_utils.h"

#define BOX_HEIGHT 10 /* Height Of About Window */
#define BOX_WIDTH 60 /* Width Of About Window */

int act_about (const MEVENT *mouse_event, const int ch, const int y, const int x, const char *name) {
    int y_pos, x_pos;

    y_pos = (y >> 1) - (BOX_HEIGHT >> 1);
    x_pos = (x >> 1) - (BOX_WIDTH >> 1);

    if (ch == KEY_MOUSE) {
        if (check_mouse_pos_box (mouse_event->y, mouse_event->x, y_pos, x_pos, BOX_HEIGHT, BOX_WIDTH)) {
            return 1;
        } else {
            return 0;
        }
    } else if (ch != OK && ch != KEY_RESIZE) {
        return 0;
    }

    print_clear_win_at (y_pos, x_pos, BOX_HEIGHT, BOX_WIDTH, name);

    attron (COLOR_PAIR (3));
    mvprintw (y_pos + 2, x_pos + 2, "TMCScan " TMCS_VERSION ": ");
    attroff (COLOR_PAIR (3));
    addstr (TMCS_DESCRIPTION);

    attron (COLOR_PAIR (3));
    mvprintw (y_pos + 3, x_pos + 2, "GitHub: ");
    attroff (COLOR_PAIR (3));
    addstr ("https://github.com/0Magenta0/TMCScan");

    attron (COLOR_PAIR (3));
    mvprintw (y_pos + 4, x_pos + 2, "Discord: ");
    attroff (COLOR_PAIR (3));
    addstr ("_Magenta_#1515");

    attron (COLOR_PAIR (3));
    mvprintw (y_pos + 5, x_pos + 2, "Mail: ");
    attroff (COLOR_PAIR (3));
    addstr ("0_magenta_0@mail.ru");

    mvprintw (y_pos + 7, x_pos + 2, "Copyright (C) 2022 _Magenta_ | Released Under GPLv3");
    refresh ();
    return 1;
}

