/*
 *  tui/src/tui.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

#include <ncurses.h>

#include <stdbool.h>
#include <unistd.h>
#include <locale.h>

#include "tui/tui_serv_info_colons.h"
#include "tui/tui_main_actions.h"
#include "tui/tui_window_quit.h"
#include "tui/tui_min_size.h"
#include "tui/tui_colors.h"
#include "tui/tui.h"

#define COLONS_COUNT 5
#define ACTIONS_COUNT 2

/* Draw the main menu box.
 * Adds the buttons and server colons
 */
static void print_main_box (const int screen_height);

/* Initialize the server colons structure */
const struct serv_info_colon colons_arr [COLONS_COUNT] = {
    { "ip",      2, 15, 'i' },
    { "port",    4, 5,  'p' },
    { "online",  6, 10, 'o' },
    { "version", 7, 27, 'v' },
    { "motd",    4, 20, 'm' }
};

/* Initialize the main menu actions structure */
struct main_action actions_arr [ACTIONS_COUNT] = {
    { "scan", 4, 's', /* window_scan */ NULL, false },
    { "quit", 4, 'q', window_quit, false }
};

bool start_tui (void) {
    setlocale (LC_ALL, "");

    initscr ();
    if (keypad (stdscr, true) == ERR) {
        return false;
    }

    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    nodelay (stdscr, true);
    curs_set (0);
    noecho ();
    raw ();

    if (has_colors ()) {
        start_color ();
    } else {
        return false;
    }

    if (can_change_color ()) {
        assume_default_colors (231, 234);
        init_pair (PAIR_GENERAL, 75, 234);
        init_pair (PAIR_POSITIVE, 112, 234);
        init_pair (PAIR_NEGATIVE, 196, 234);
    } else {
        init_pair (PAIR_GENERAL, COLOR_CYAN, COLOR_BLACK);
        init_pair (PAIR_POSITIVE, COLOR_GREEN, COLOR_BLACK);
        init_pair (PAIR_NEGATIVE, COLOR_RED, COLOR_BLACK);
    }

    return true;
}

void end_tui (void) {
    endwin ();
}

void show_menu (void) {
    int ch, screen_height, screen_width, counter;
    MEVENT mouse_event;

    /* Initial TUI state draw. */
    getmaxyx (stdscr, screen_height, screen_width);
    if (screen_height < MIN_SCR_LINES || screen_width < MIN_SCR_COLS) {
        is_quite_large = false;
        print_min_size (screen_height, screen_width);
    } else {
        print_main_box (screen_height);
    }

    /* Main loop */
    while ((ch = getch ()) && is_open) {
        if (ch == ERR ||
            (is_quite_large == false && ch != KEY_RESIZE) ||
            (ch == KEY_MOUSE && getmouse (&mouse_event) == ERR)) {
            goto _key_loop_end; /* Repeat the loop if some error is handled. */
        }

        /* Terminal resize handle. */
        if (ch == KEY_RESIZE) {
            getmaxyx (stdscr, screen_height, screen_width);
            if (screen_height < MIN_SCR_LINES || screen_width < MIN_SCR_COLS) {
                is_quite_large = false;
                print_min_size (screen_height, screen_width);
            } else {
                is_quite_large = true;
                print_main_box (screen_height);
                for (counter = 0; counter < ACTIONS_COUNT; ++counter) {
                    if (actions_arr [counter].is_enabled == true) {
                        if (actions_arr [counter].func (KEY_RESIZE, screen_height, screen_width, actions_arr [counter].name, NULL) == false) {
                            actions_arr [counter].is_enabled = false;
                            print_main_box (screen_height);
                        }
                    }
                }
            } goto _key_loop_end; /* Don't try any other handlers. */
        }

        /* Check if the any window is enabled. */
        for (counter = 0; counter < ACTIONS_COUNT; ++counter) {
            if (actions_arr [counter].is_enabled == true) {
                if (actions_arr [counter].func (ch, screen_height, screen_width, actions_arr [counter].name, &mouse_event) == false) {
                    actions_arr [counter].is_enabled = false;
                    print_main_box (screen_height);
                } goto _key_loop_end;
            }
        }

        /* Handle keybinds that enable any window. */
        for (counter = 0; counter < ACTIONS_COUNT; ++counter) {
            if (ch == actions_arr [counter].bind) {
                actions_arr [counter].is_enabled = true;
                actions_arr [counter].func (OK, screen_height, screen_width, actions_arr [counter].name, NULL);
                goto _key_loop_end;
            }
        }

        /* Handle mouse clicks on the action buttons */
        if (ch == KEY_MOUSE && (counter = handle_mouse_on_main_actions (&mouse_event, screen_height, actions_arr, ACTIONS_COUNT)) < ACTIONS_COUNT) {
            actions_arr [counter].is_enabled = true;
            actions_arr [counter].func (OK, screen_height, screen_width, actions_arr [counter].name, NULL);
            // goto _key_loop_end;
        }

_key_loop_end:
        if (ch == ERR) {
            usleep (60000); /* CPU Runtime optimization. */
        }
    }
}

static void print_main_box (const int screen_height) {
    clear ();
    box (stdscr, 0, 0);
    print_serv_info_colons (colons_arr, COLONS_COUNT);
    print_main_actions (screen_height, actions_arr, ACTIONS_COUNT);

    refresh ();
}

