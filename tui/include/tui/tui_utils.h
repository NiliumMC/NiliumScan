/*
 *  tui/include/tui/tui_utils.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include <stdbool.h>

#include "tui/tui_button.h"

#define MAX_TUI_KEY_BINDS 8

enum item_change_direction {
    ICD_UP,
    ICD_DOWN,
    ICD_LEFT,
    ICD_RIGHT
};

enum item_type {
    BUTTON_TYPE,
    TEXTFIELD_TYPE /* TODO */
};

struct item_change_direction_binding {
    const int bind;
    const bool is_for_buttons;
    const bool is_for_textfields;
};

struct item_change_direction_bindings {
    const enum item_change_direction direction;
    const int array_size;
    const struct item_change_direction_binding binds_arr [MAX_TUI_KEY_BINDS];
};

extern const struct item_change_direction_bindings binds_arr [4];

void print_clear_win_at (const int, const int, const int, const int, const char * const);
int change_item (const int, const int, const int, const enum item_change_direction, const struct tui_button [], const int);
bool check_bind (const int, const enum item_type, const struct item_change_direction_bindings [], const int, enum item_change_direction *);
const struct tui_button * get_button_by_id (const int, const struct tui_button [], const int);
enum item_type get_item_type_by_id (const int, const struct tui_button [], const int);
bool is_enter_key (const int);
bool is_mouse_click_out_window (const MEVENT * const, const int, const int, const int, const int);

