/*
 *  tui/src/tui_button.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_button.h"

static void print_button (const int, const int, const struct tui_button *);

void print_buttons (const int window_y_pos, const int window_x_pos, const struct tui_button button_arr [], const int array_size) {
    int counter;

    for (counter = 0; counter < array_size; ++counter) {
        print_button (window_y_pos, window_x_pos, &button_arr [counter]);
    }
}

static void print_button (const int window_y_pos, const int window_x_pos, const struct tui_button *button) {
    mvprintw (window_y_pos + button->y_pos, window_x_pos + button->x_pos, "<%s>", button->name);
}

