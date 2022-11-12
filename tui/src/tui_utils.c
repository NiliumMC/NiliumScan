/*
 *  tui/src/tui_utils.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_colors.h"
#include "tui/tui_utils.h"

const struct item_change_direction_bindings binds_arr [4] = {
    { ICD_UP, 2, {
        { 'k',    true, false },
        { KEY_UP, true, true }
    } },

    { ICD_DOWN, 2, {
        { 'j',      true, false },
        { KEY_DOWN, true, true }
    } },

    { ICD_LEFT, 2, {
        { 'h',      true, false },
        { KEY_LEFT, true, true }
    } },

    { ICD_RIGHT, 2, {
        { 'l',       true, false },
        { KEY_RIGHT, true, true }
    } },
};

void print_clear_win_at (const int y_pos, const int x_pos, const int height, const int width, const char * const name) {
    int counter_height, counter_width;

    for (counter_height = 0; counter_height < height; ++counter_height) {
        for (counter_width = 0; counter_width < width; ++counter_width) {
            mvaddch (y_pos + counter_height, x_pos + counter_width, ' ');
        }
    }

    mvaddch (y_pos, x_pos, ACS_ULCORNER);
    mvaddch (y_pos, x_pos + width - 1, ACS_URCORNER);
    mvaddch (y_pos + height - 1, x_pos, ACS_LLCORNER);
    mvaddch (y_pos + height - 1, x_pos + width - 1, ACS_LRCORNER);
    mvhline (y_pos, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos, ACS_VLINE, height - 2);
    mvhline (y_pos + height - 1, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos + width - 1, ACS_VLINE, height - 2);

    mvaddch (y_pos, x_pos + 1, ACS_URCORNER);
    attron (COLOR_PAIR (PAIR_GENERAL) | A_ITALIC);
    addstr (name);
    standend ();
    addch (ACS_ULCORNER);
}

int change_item (const int window_y_pos, const int window_x_pos, const int current_item_id, const enum item_change_direction direction, const struct tui_button button_arr [], const int buttons_array_size) {
    const struct tui_button *tmp_button_p;
    int counter;

    if ((tmp_button_p = get_button_by_id (current_item_id, button_arr, buttons_array_size)) != NULL) {
        print_button (window_y_pos, window_x_pos, tmp_button_p, false);
    }

    for (counter = 0; counter < buttons_array_size; ++counter) {
        if (current_item_id == button_arr [counter].element_id) {
            switch (direction) {
                case ICD_UP: {
                    if ((tmp_button_p = get_button_by_id (button_arr [counter].up_element_id, button_arr, buttons_array_size)) != NULL) {
                        print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                        return tmp_button_p->element_id;
                    } break;
                }

                case ICD_DOWN: {
                    if ((tmp_button_p = get_button_by_id (button_arr [counter].down_element_id, button_arr, buttons_array_size)) != NULL) {
                        print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                        return tmp_button_p->element_id;
                    } break;
                }

                case ICD_LEFT: {
                    if ((tmp_button_p = get_button_by_id (button_arr [counter].left_element_id, button_arr, buttons_array_size)) != NULL) {
                        print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                        return tmp_button_p->element_id;
                    } break;
                }

                case ICD_RIGHT: {
                    if ((tmp_button_p = get_button_by_id (button_arr [counter].right_element_id, button_arr, buttons_array_size)) != NULL) {
                        print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                        return tmp_button_p->element_id;
                    }
                }
            }
        }
    }

    return 0;
}

bool check_bind (const int ch, const enum item_type type, const struct item_change_direction_bindings bindings_arr [], const int array_size, enum item_change_direction *direction) {
    int counter_array, counter_bindings;

    for (counter_array = 0; counter_array < array_size; ++counter_array) {
        for (counter_bindings = 0; counter_bindings < bindings_arr [counter_array].array_size; ++counter_bindings) {
            if (ch == bindings_arr [counter_array].binds_arr [counter_bindings].bind) {
                *direction = bindings_arr [counter_array].direction;
                switch (type) {
                    case BUTTON_TYPE: {
                        if (bindings_arr [counter_array].binds_arr [counter_bindings].is_for_buttons) {
                            return true;
                        } break;
                    }

                    case TEXTFIELD_TYPE: {
                        if (bindings_arr [counter_array].binds_arr [counter_bindings].is_for_textfields) {
                            return true;
                        } break;
                    }

                    default: return false;
                }
            }
        }
    }

    return false;
}

const struct tui_button * get_button_by_id (const int id, const struct tui_button button_arr [], const int buttons_array_size) {
    int counter;

    for (counter = 0; counter < buttons_array_size; ++counter) {
        if (button_arr [counter].element_id == id) {
            return &button_arr [counter];
        }
    }

    return NULL;
}

enum item_type get_item_type_by_id (const int id, const struct tui_button button_arr [], const int buttons_array_size) {
    int counter;

    for (counter = 0; counter < buttons_array_size; ++counter) {
        if (button_arr [counter].element_id == id) {
            return BUTTON_TYPE;
        }
    }

    /* TODO: Textfields */

    return BUTTON_TYPE;
}

bool is_enter_key (const int ch) {
    if (ch == 0x0A || ch == KEY_ENTER) {
        return true;
    } else {
        return false;
    }
}

