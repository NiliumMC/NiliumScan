/*****************************************************************************
 *
 *    tui/src/tui.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include "tui/tui_params.h"
#include "tui/tui_utils.h"
#include "tui/tui_acts.h"
#include "tui/tui_scan.h"
#include "tui/tui_quit.h"
#include "tui/tui.h"

#include "scan/scan.h"

#define MIN_LINES 15
#define MIN_COLS 50
#define PARAMS_COUNT 5
#define ACTS_COUNT 2

void print_main_box (const int, const int);
void print_servers (const unsigned int);
void print_server (const unsigned int, const unsigned int);

int ltc_menu_pos_x; /* Left-Top corner of menu position by horizontal */

const struct param params_list [PARAMS_COUNT] = { { "ip", 15, 'i' },
                                                  { "port", 5, 'p' },
                                                  { "online", 10, 'o' },
                                                  { "version", 27, 'v' },
                                                  { "motd", 20 /* Limit ~59 */, 'm' } };

struct action acts_list [ACTS_COUNT] = { { "scan", 4, act_scan, 's', 0 },
                                         { "quit", 4, act_quit, 'q', 0 } };

int ini_curses (void) {
    setlocale (LC_ALL, "");
    initscr ();
    if (keypad (stdscr, 1) == ERR)
        return 0;

    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, 0);
    nodelay (stdscr, 1);
    curs_set (0);
    noecho ();
    raw ();

    if (has_colors ())
        start_color ();
    else
        return 0;

    /* Zero color pair is initialized by start_color () */
    if (can_change_color ()) {
        assume_default_colors (250, 16);
        init_pair (1, 196, 16);
        init_pair (2, 112, 16);
        init_pair (3, 80, 16);
    } else {
        init_pair (1, COLOR_RED, COLOR_BLACK);
        init_pair (2, COLOR_GREEN, COLOR_BLACK);
        init_pair (3, COLOR_CYAN, COLOR_BLACK);
    }

    return 1;
}

void show_menu (void) {
    MEVENT mouse_event;
    register int i, ch;
    int y, x;

    getmaxyx (stdscr, y, x);
    if (y < MIN_LINES || x < MIN_COLS)
        print_min_size (MIN_LINES, MIN_COLS, y, x);
    else
        print_main_box (y, x);

    while ((ch = getch ()) && is_open) {
        if (scan_status == scan_status_scanning) {
            if (ch == KEY_RESIZE) {
                getmaxyx (stdscr, y, x);
            } if (y < MIN_LINES || x < MIN_COLS) {
                    print_min_size (MIN_LINES, MIN_COLS, y, x);
                    refresh ();
                    goto _key_loop_end; /* TODO: Endless loop fix */
            }

            print_main_box (y, x);
            acts_list [0].func (0, OK, y, x, acts_list [0].name);
            goto _key_loop_end;
        }

        if (ch == ERR)
            goto _key_loop_end;

        if (ch == KEY_MOUSE && getmouse (&mouse_event) == ERR)
            goto _key_loop_end;

        if (ch == KEY_RESIZE) {
            getmaxyx (stdscr, y, x);
            if (y < MIN_LINES || x < MIN_COLS) {
                print_min_size (MIN_LINES, MIN_COLS, y, x);
                continue;
            }

            print_main_box (y, x);
            for (i = 0; i < ACTS_COUNT; ++i)
                if (acts_list [i].enabled)
                    acts_list [i].enabled = acts_list [i].func (0, ch, y, x, acts_list [i].name);

        } else {
            if (y < MIN_LINES || x < MIN_COLS) {
                print_min_size (MIN_LINES, MIN_COLS, y, x);
                continue;
            } for (i = 0; i < ACTS_COUNT; ++i) {
                if (acts_list [i].enabled) {
                    if (!(acts_list [i].enabled = acts_list [i].func (ch == KEY_MOUSE ? &mouse_event : 0, ch, y, x, acts_list [i].name)))
                        print_main_box (y, x);
                    goto _key_loop_end;
                }
            } for (i = 0; i < ACTS_COUNT; ++i) {
                if (acts_list [i].bind == ch) {
                    acts_list [i].enabled = 1;
                    acts_list [i].func (0, OK, y, x, acts_list [i].name);
                }
            }
        }

_key_loop_end:
        if (ch == ERR) {
            usleep (60000);
        } if (scan_status == scan_status_scanning) {
            sleep (1);
        }
    }
}

void print_main_box (const int y, const int x) {
    int i, x_pos;

    clear ();
    box (stdscr, 0, 0);
    for (i = 0, x_pos = 2; i < PARAMS_COUNT; x_pos += 1 + params_list [i].len, ++i)
        print_param (x, x_pos, &params_list [i]);

    for (i = 0, x_pos = 2; i < ACTS_COUNT; x_pos += 2 + acts_list [i].len, ++i)
        print_act (y - 1, x, x_pos, &acts_list [i]);

    print_servers (y + 1);

    refresh ();
}

void print_servers (const unsigned int y) {
    unsigned int i;

    if (scan_status != scan_status_scanning && serv_items_array_len) {
        mvprintw (0, 0, "%d", serv_items_array_len);
        for (i = 0; i < serv_items_array_len && i < y - 3; ++i) {
            print_server (i + 1, i);
        }
    }
}

void print_server (const unsigned int y, const unsigned int i) {
    mvprintw (y, 2, "%s       %d %d/%d        %s                        %s",
              serv_items_array [i].ip,
              serv_items_array [i].port,
              serv_items_array [i].online,
              serv_items_array [i].slots,
              serv_items_array [i].version,
              serv_items_array [i].motd);
}

void fin_curses (void) {
    endwin ();
}

