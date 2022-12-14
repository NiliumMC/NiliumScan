/*
 *  tui/src/tui_utils.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <ncurses.h>

#include "tui/tui_colors.h"
#include "tui/tui_utils.h"

/* Initialization of array with bindings
 * for 4 directions.
 */
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

void print_clear_win_at (const int y_pos, const int x_pos,
        const int height, const int width, const char * const name) {
    int counter_height, counter_width;

    /* Clear the window area. */
    for (counter_height = 0; counter_height < height; ++counter_height) {
        for (counter_width = 0; counter_width < width; ++counter_width) {
            mvaddch (y_pos + counter_height, x_pos + counter_width, ' ');
        }
    }

    /* Draw the window border. */
    mvaddch (y_pos, x_pos, ACS_ULCORNER);
    mvaddch (y_pos, x_pos + width - 1, ACS_URCORNER);
    mvaddch (y_pos + height - 1, x_pos, ACS_LLCORNER);
    mvaddch (y_pos + height - 1, x_pos + width - 1, ACS_LRCORNER);
    mvhline (y_pos, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos, ACS_VLINE, height - 2);
    mvhline (y_pos + height - 1, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos + width - 1, ACS_VLINE, height - 2);

    /* Add the window title. */
    mvaddch (y_pos, x_pos + 1, ACS_URCORNER);
    attron (COLOR_PAIR (PAIR_GENERAL) | A_ITALIC);
    addstr (name);
    standend ();
    addch (ACS_ULCORNER);
}

int directional_change_item (const int window_y_pos, const int window_x_pos,
        const int current_item_id, const enum item_change_direction direction,
        const struct tui_button buttons_arr [], const int buttons_array_size,
        const struct tui_textfield textfields_arr [], const int textfields_array_size) {
    const struct tui_textfield *tmp_textfield_p;
    const struct tui_button *tmp_button_p;
    int counter;

    /* Redraw the old item. */
    if ((tmp_button_p = get_button_by_id (current_item_id, buttons_arr, buttons_array_size)) != NULL) {
        print_button (window_y_pos, window_x_pos, tmp_button_p, false);
    } else if ((tmp_textfield_p = get_textfield_by_id (current_item_id, textfields_arr, textfields_array_size)) != NULL) {
        print_textfield (window_y_pos, window_x_pos, tmp_textfield_p, false);
    }

    /* Find and draw the new one. */
    for (counter = 0; counter < buttons_array_size; ++counter) {
        if (current_item_id == buttons_arr [counter].element_id) {
            if ((tmp_button_p = get_button_by_id (direction == ICD_UP ? buttons_arr [counter].up_element_id :
                            direction == ICD_DOWN ? buttons_arr [counter].down_element_id :
                            direction == ICD_LEFT ? buttons_arr [counter].left_element_id :
                            buttons_arr [counter].right_element_id,
                            buttons_arr, buttons_array_size)) != NULL) {
                print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                return tmp_button_p->element_id;
            } else if ((tmp_textfield_p = get_textfield_by_id (direction == ICD_UP ? buttons_arr [counter].up_element_id :
                            direction == ICD_DOWN ? buttons_arr [counter].down_element_id :
                            direction == ICD_LEFT ? buttons_arr [counter].left_element_id :
                            buttons_arr [counter].right_element_id,
                            textfields_arr, textfields_array_size)) != NULL) {
                print_textfield (window_y_pos, window_x_pos, tmp_textfield_p, true);
                return tmp_textfield_p->element_id;
            }
        }
    } for (counter = 0; counter < textfields_array_size; ++counter) {
        if (current_item_id == textfields_arr [counter].element_id) {
            if ((tmp_button_p = get_button_by_id (direction == ICD_UP ? textfields_arr [counter].up_element_id :
                            direction == ICD_DOWN ? textfields_arr [counter].down_element_id :
                            direction == ICD_LEFT ? textfields_arr [counter].left_element_id :
                            textfields_arr [counter].right_element_id,
                            buttons_arr, buttons_array_size)) != NULL) {
                print_button (window_y_pos, window_x_pos, tmp_button_p, true);
                return tmp_button_p->element_id;
            } else if ((tmp_textfield_p = get_textfield_by_id (direction == ICD_UP ? textfields_arr [counter].up_element_id :
                            direction == ICD_DOWN ? textfields_arr [counter].down_element_id :
                            direction == ICD_LEFT ? textfields_arr [counter].left_element_id :
                            textfields_arr [counter].right_element_id,
                            textfields_arr, textfields_array_size)) != NULL) {
                print_textfield (window_y_pos, window_x_pos, tmp_textfield_p, true);
                return tmp_textfield_p->element_id;
            }
        }
    }

    return 0;
}

bool check_bind (const int ch, const enum item_type type,
        const struct item_change_direction_bindings bindings_arr [],
        const int array_size, enum item_change_direction *direction) {
    int counter_array, counter_bindings;

    for (counter_array = 0; counter_array < array_size; ++counter_array) {
        for (counter_bindings = 0; counter_bindings < bindings_arr [counter_array].array_size; ++counter_bindings) {
            if (ch == bindings_arr [counter_array].binds_arr [counter_bindings].bind) {
                *direction = bindings_arr [counter_array].direction;
                switch (type) {
                    case BUTTON_TYPE:
                        if (bindings_arr [counter_array].binds_arr [counter_bindings].is_for_buttons) {
                            return true;
                        } break;

                    case TEXTFIELD_TYPE:
                        if (bindings_arr [counter_array].binds_arr [counter_bindings].is_for_textfields) {
                            return true;
                        } break;

                    default: return false;
                }
            }
        }
    }

    return false;
}

enum item_type get_item_type_by_id (const int id,
        const struct tui_button buttons_arr [], const int buttons_array_size,
        const struct tui_textfield textfields_arr [], const int textfields_array_size) {
    register int counter;

    if (buttons_arr) {
        for (counter = 0; counter < buttons_array_size; ++counter) {
            if (buttons_arr [counter].element_id == id) {
                return BUTTON_TYPE;
            }
        }
    } if (textfields_arr) {
        for (counter = 0; counter < textfields_array_size; ++counter) {
            if (textfields_arr [counter].element_id == id) {
                return TEXTFIELD_TYPE;
            }
        }
    }

    return BUTTON_TYPE;
}

bool is_enter_key (const int ch) {
    if (ch == 0x0A || ch == KEY_ENTER) {
        return true;
    } else {
        return false;
    }
}

bool is_mouse_click_out_window (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const int window_height, const int window_width) {
    if ((mouse_event->y < window_y_pos || mouse_event->y >= window_y_pos + window_height ||
                mouse_event->x < window_x_pos || mouse_event->x >= window_x_pos + window_width)) {
        return true;
    }

    return false;
}

bool is_mouse_click_on_item (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos, int * new_item_id,
        const struct tui_button buttons_arr [], const int buttons_array_size,
        const struct tui_textfield textfields_arr [], const int textfields_array_size) {
    register int counter;

    if (buttons_arr) {
        for (counter = 0; counter < buttons_array_size; ++counter) {
            if (is_mouse_click_on_button (mouse_event, window_y_pos, window_x_pos,
                    &buttons_arr [counter])) {
                *new_item_id = buttons_arr [counter].element_id;

                return true;
            }
        }
    } if (textfields_arr) {
        for (counter = 0; counter < textfields_array_size; ++counter) {
            if (is_mouse_click_on_textfield (mouse_event, window_y_pos, window_x_pos,
                    &textfields_arr [counter])) {
                *new_item_id = textfields_arr [counter].element_id;

                return true;
            }
        }
    }

    return false;
}

void change_item (const int window_y_pos, const int window_x_pos,
        const int old_item_id, const int new_item_id,
        const struct tui_button buttons_arr [], const int buttons_array_size,
        const struct tui_textfield textfields_arr [], const int textfields_array_size) {
    register int counter;

    if (buttons_arr) {
        for (counter = 0; counter < buttons_array_size; ++counter) {
            if (old_item_id == buttons_arr [counter].element_id) {
                print_button (window_y_pos, window_x_pos, &buttons_arr [counter], false);
            } else if (new_item_id == buttons_arr [counter].element_id) {
                print_button (window_y_pos, window_x_pos, &buttons_arr [counter], true);
            }
        }
    } if (textfields_arr) {
        for (counter = 0; counter < textfields_array_size; ++counter) {
            if (old_item_id == textfields_arr [counter].element_id) {
                print_textfield (window_y_pos, window_x_pos, &textfields_arr [counter], false);
            } else if (new_item_id == textfields_arr [counter].element_id) {
                print_textfield (window_y_pos, window_x_pos, &textfields_arr [counter], true);
            }
        }
    }
}

bool is_mouse_left_click (const MEVENT *mouse_event) {
    return mouse_event->bstate & BUTTON1_CLICKED;
}

bool is_mouse_left_double_click (const MEVENT *mouse_event) {
    return mouse_event->bstate & BUTTON1_DOUBLE_CLICKED;
}

