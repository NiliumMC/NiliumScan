/*
 *  tui/include/tui/tui_utils.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#include <ncurses.h>

#include "tui/tui_button.h"

/* This number is the maximum
 * bindings array size for only
 * one direction.
 */
#define MAX_TUI_KEY_BINDS 8

/* List of directions in which
 * neighboring elements can be located.
 */
enum item_change_direction {
    ICD_UP,
    ICD_DOWN,
    ICD_LEFT,
    ICD_RIGHT
};

/* List of window element types. */
enum item_type {
    BUTTON_TYPE,
    TEXTFIELD_TYPE /* TODO */
};

/* Structure with binding that change element. */
struct item_change_direction_binding {
    const int bind;
    const bool is_for_buttons;
    const bool is_for_textfields;
};

/* Struct with list of bindings
 * for only one direction.
 */
struct item_change_direction_bindings {
    const enum item_change_direction direction;
    const int array_size;
    const struct item_change_direction_binding binds_arr [MAX_TUI_KEY_BINDS];
};

/* Array declaration with bindings
 * for 4 directions.
 */
extern const struct item_change_direction_bindings binds_arr [4];

/* Draw empty window at some position.
 * Sets the title for the window.
 */
void print_clear_win_at (const int y_pos, const int x_pos,
        const int height, const int width, const char * const name);

/* Selects the next element at some direction.
 * Redraw the elements.
 *
 * Returns the ID of the new element. */
int directional_change_item (const int window_y_pos, const int window_x_pos,
        const int current_item_id, const enum item_change_direction direction,
        const struct tui_button button_arr [], const int array_size);

/* Checks if the some pressed
 * character is the keybinding.
 * Sets the direction of the new element.
 *
 * Returns true if character is
 * found, otherwise false.
 */
bool check_bind (const int ch, const enum item_type type,
        const struct item_change_direction_bindings bindings_arr [],
        const int array_size, enum item_change_direction *direction);

/* Searches the button element by ID.
 *
 * If found, returns the pointer
 * to the element, otherwise NULL.
 */
const struct tui_button * get_button_by_id (const int id,
        const struct tui_button buttons_arr [], const int array_size);

/* Iterate by all window elements
 * and returns the type of the
 * provided element ID.
 */
enum item_type get_item_type_by_id (const int id,
        const struct tui_button button_arr [], const int buttons_array_size);

/* Returns true if the provided
 * key is the ENTER.
 */
bool is_enter_key (const int ch);

/* Checks if the mouse click
 * was outside the window.
 *
 * Returns true if it's outside,
 * otherwise false.
 */
bool is_mouse_click_out_window (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos,
        const int window_height, const int window_width);

/* Checks if the mouse click
 * was on the some TUI element.
 *
 * Writes new element id
 * to the new_item_id.
 *
 * Returns true if it is,
 * otherwise false.
 */
bool is_mouse_click_on_item (const MEVENT * const mouse_event,
        const int window_y_pos, const int window_x_pos, int *new_item_id,
        const struct tui_button button_arr [], const int buttons_array_size);

/* Highlights new window element */
void change_item (const int window_y_pos, const int window_x_pos,
        const int old_item_id, const int new_item_id,
        const struct tui_button buttons_arr [], const int buttons_array_size);

/* Check if LMB is clicked.
 *
 * Returns true if so,
 * otherwise false.
 */
bool is_mouse_left_click (const MEVENT *mouse_event);

/* Check if LMB is double-clicked.
 *
 * Returns true if so,
 * otherwise false.
 */
bool is_mouse_left_double_click (const MEVENT *mouse_event);

