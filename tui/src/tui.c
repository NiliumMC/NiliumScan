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
#include "tui/tui_colors.h"
#include "tui/tui.h"

#define MIN_SCR_LINES 24
#define MIN_SCR_COLS 80

#define COLONS_COUNT 5
#define ACTIONS_COUNT 2

void print_main_box (const int);
void print_min_size (const int, const int);

const struct serv_info_colon colons_arr [COLONS_COUNT] = {
    { "ip", 2, 15, 'i' },
    { "port", 4, 5, 'p' },
    { "online", 6, 10, 'o' },
    { "version", 7, 27, 'v' },
    { "motd", 4, 20, 'm' }
};

const struct main_action actions_arr [ACTIONS_COUNT] = {
    { "scan", 4, 's', /* window_scan */ NULL, false },
    { "quit", 4, 'q', /* window_quit */ NULL, false }
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
        init_pair (pair_general, 75, 234);
        init_pair (pair_positive, 112, 234);
        init_pair (pair_negative, 196, 234);
    } else {
        init_pair (pair_general, COLOR_CYAN, COLOR_BLACK);
        init_pair (pair_positive, COLOR_GREEN, COLOR_BLACK);
        init_pair (pair_negative, COLOR_RED, COLOR_BLACK);
    }

    return true;
}

void end_tui (void) {
    endwin ();
}

void show_menu (void) {
    int ch, y, x;

    getmaxyx (stdscr, y, x);
    if (y < MIN_SCR_LINES || x < MIN_SCR_COLS) {
        print_min_size (y, x);
    } else {
        print_main_box (y);
    }

    while (true) {
        ch = getch ();
        if (ch == ERR) {
            goto _key_loop_end;
        }

        if (ch == KEY_RESIZE) {
            getmaxyx (stdscr, y, x);
            if (y < MIN_SCR_LINES || x < MIN_SCR_COLS) {
                print_min_size (y, x);
            } else {
                print_main_box (y);
            }
        }

_key_loop_end:
        if (ch == ERR) {
            usleep (60000);
        }
    }
}

void print_main_box (const int y) {
    clear ();
    box (stdscr, 0, 0);
    print_serv_info_colons (COLONS_COUNT, colons_arr);
    print_main_actions (y, ACTIONS_COUNT, actions_arr);

    refresh ();
}

void print_min_size (const int y, const int x) {
    int y_pos, x_pos;

    y_pos = (y >> 1) - 2;
    x_pos = (x >> 1) - 16;

    clear ();

    mvprintw (y_pos++, x_pos, "Your terminal is too small!");

    mvprintw (y_pos++, x_pos, "      Lines: %d/", MIN_SCR_LINES);
    attron (COLOR_PAIR ((y >= MIN_SCR_LINES) ? pair_positive : pair_negative));
    printw ("%d", y);
    standend ();

    mvprintw (y_pos, x_pos, "      Cols: %d/", MIN_SCR_COLS);
    attron (COLOR_PAIR ((x >= MIN_SCR_COLS) ? pair_positive : pair_negative));
    printw ("%d", x);
    standend ();

    refresh ();
}

