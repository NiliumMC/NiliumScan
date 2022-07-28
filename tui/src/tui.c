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

#include "tui/tui_filter.h"
#include "tui/tui_params.h"
#include "tui/tui_utils.h"
#include "tui/tui_about.h"
#include "tui/tui_acts.h"
#include "tui/tui_scan.h"
#include "tui/tui_quit.h"
#include "tui/tui.h"

#include "scan/scan.h"

#define MIN_LINES 15 /* Minimum Count Of Screen's Lines */
#define MIN_COLS 80 /* Minimum Count Of Screen's Cols */

#define PARAMS_COUNT 5 /* Count Of Params At Main Window */
#define ACTS_COUNT 3 /* Count Of Acts At Main Window */

void print_main_box (const int, const int);
void print_servers (const int, const int, const int);
void print_server (const int, const int, const int, const char);
void print_current_item_num (const int, const int);

int ltc_menu_pos_x; /* Left-Top corner of menu position by horizontal */

int current_serv_item, /* Selected Now Servers List Item */
    items_shift; /* Servers List Items Offset */

/* Params List */
const struct param params_list [PARAMS_COUNT] = { { "ip", 2, 15, 'i' },
                                                  { "port", 4, 5, 'p' },
                                                  { "online", 6, 10, 'o' },
                                                  { "version", 7, 27, 'v' },
                                                  { "motd", 4, 20, 'm' } };

/* Acts List */
struct action acts_list [ACTS_COUNT] = { { "scan", 4, act_scan, 's', 0 },
                                         { "quit", 4, act_quit, 'q', 0 },
                                         { "about", 5, act_about, 'a', 0 } };

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

    if (can_change_color ()) {
        assume_default_colors (231, 16);
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
    register int y, x, i, ch;

    getmaxyx (stdscr, y, x);
    if (y < MIN_LINES || x < MIN_COLS) {
        print_min_size (MIN_LINES, MIN_COLS, y, x);
    } else {
        print_main_box (y, x);
        print_current_item_num (y, x);
    }

    while ((ch = getch ()) && is_open) {
        /* Check If Scan Is Ended And Draw Servers */
        if (scan_status == scan_status_scanning) {
            if (!is_scanning) {
                items_shift = 0;
                current_serv_item = 0;
                scan_status = scan_status_end;
                copy_serv_list_to_array ();
                free (scan_threads);
            } if (ch == KEY_RESIZE) {
                getmaxyx (stdscr, y, x);
            } if (y < MIN_LINES || x < MIN_COLS) {
                print_min_size (MIN_LINES, MIN_COLS, y, x);
                refresh ();
                goto _key_loop_end;
            }

            print_main_box (y, x);
            print_servers (y - 2, x - 4, OK);
            acts_list [0].func (0, OK, y, x, acts_list [0].name);
            goto _key_loop_end;
        }

        if (ch == ERR) {
            goto _key_loop_end;
        } if (ch == KEY_MOUSE && getmouse (&mouse_event) == ERR) {
            goto _key_loop_end;
        }

        if (ch == KEY_RESIZE) {
            getmaxyx (stdscr, y, x);
            if (y < MIN_LINES || x < MIN_COLS) {
                print_min_size (MIN_LINES, MIN_COLS, y, x);
                continue;
            }

            if (is_entering_filter) {
                ch = OK;
            }

            if (is_filtering) {
                if (filtered_indexes_array_len > (y - 2)) {
                    if (current_filtered_serv_item - filtered_items_shift + 1 > y - 2) {
                        filtered_items_shift += (current_filtered_serv_item - filtered_items_shift + 1) - (y - 2);
                    } else if (filtered_items_shift && filtered_indexes_array_len - filtered_items_shift < y - 2) {
                        filtered_items_shift -= (y - 2) - (filtered_indexes_array_len - filtered_items_shift);
                    }
                } else {
                    filtered_items_shift = 0;
                }
            } else {
                if (serv_items_array_len > (y - 2)) {
                    if (current_serv_item - items_shift + 1 > y - 2) {
                        items_shift += (current_serv_item - items_shift + 1) - (y - 2);
                    } else if (items_shift && serv_items_array_len - items_shift < y - 2) {
                        items_shift -= (y - 2) - (serv_items_array_len - items_shift);
                    }
                } else {
                    items_shift = 0;
                }
            }

            print_main_box (y, x);
            print_servers (y - 2, x - 4, OK);
            for (i = 0; i < ACTS_COUNT; ++i) {
                if (acts_list [i].enabled) {
                    acts_list [i].enabled = acts_list [i].func (0, ch, y, x, acts_list [i].name);
                }
            }
        }

        if (y < MIN_LINES || x < MIN_COLS) {
            print_min_size (MIN_LINES, MIN_COLS, y, x);
            continue;
        }

        /* Params & Acts Rendering */
        if (!is_entering_filter) {
            for (i = 0; i < ACTS_COUNT; ++i) {
                if (acts_list [i].enabled) {
                    if (!(acts_list [i].enabled = acts_list [i].func (ch == KEY_MOUSE ? &mouse_event : 0, ch, y, x, acts_list [i].name))) {
                        print_main_box (y, x);
                        print_servers (y - 2, x - 4, OK);
                    } goto _key_loop_end;
                }
            } for (i = 0; i < ACTS_COUNT; ++i) {
                if (acts_list [i].bind == ch) {
                    acts_list [i].enabled = 1;
                    acts_list [i].func (0, OK, y, x, acts_list [i].name);
                    goto _key_loop_end;
                }
            } for (i = 0; i < PARAMS_COUNT; ++i) {
                if (params_list [i].bind == ch) {
                    sort_servers (i);
                    print_main_box (y, x);
                    print_servers (y - 2, x - 4, OK);
                    goto _key_loop_end;
                }
            }
        }

        /* Mouse Keys Handler */
        else if (check_mouse_click (&mouse_event)) {
            if (is_entering_filter) {
                if (check_mouse_pos_filter_ok_button (x, 69, &mouse_event)) {
                    filter_key_handler (KEY_ENTER);
                } else if (check_mouse_pos_filter_dl_button (x, 69, &mouse_event)) {
                    disable_entering_filter ();
                    drop_filter ();
                } else {
                    disable_entering_filter ();
                }

                print_main_box (y, x);
                print_servers (y - 2, x - 4, OK);
            } else if ((i = check_mouse_pos_params (&mouse_event, params_list, PARAMS_COUNT)) >= 0) {
                sort_servers (i);
                print_main_box (y, x);
                print_servers (y - 2, x - 4, OK);
            } else if ((i = check_mouse_pos_acts (y - 1, &mouse_event, acts_list, ACTS_COUNT)) >= 0) {
                acts_list [i].enabled = 1;
                acts_list [i].func (0, OK, y, x, acts_list [i].name);
            } else if ((i = check_mouse_pos_serv_list (y - 2, x - 2, items_shift, &mouse_event)) >= 0) {
                if (is_filtering) {
                    print_server (current_filtered_serv_item - filtered_items_shift + 1, x - 4, filtered_indexes_array [current_filtered_serv_item], 0);
                    current_filtered_serv_item = i;
                    print_server (current_filtered_serv_item - filtered_items_shift + 1, x - 4, filtered_indexes_array [i], 1);
                } else {
                    print_server (current_serv_item - items_shift + 1, x - 4, current_serv_item, 0);
                    current_serv_item = i;
                    print_server (current_serv_item - items_shift + 1, x - 4, current_serv_item, 1);
                }

                print_current_item_num (y, x);
            } else if (!is_entering_filter) {
                if (is_filtering && check_mouse_pos_filter_dl_button (x, 69, &mouse_event)) {
                    drop_filter ();
                    print_main_box (y, x);
                    print_servers (y - 2, x - 4, OK);
                    goto _key_loop_end;
                } if ((check_mouse_click (&mouse_event) || check_mouse_double_click (&mouse_event)) && check_mouse_pos_filter (&mouse_event)) {
                    is_entering_filter = 1;
                    print_main_box (y, x);
                    print_servers (y - 2, x - 4, OK);
                    goto _key_loop_end;
                }
            }
        } else if (check_mouse_double_click (&mouse_event)) {
            if ((i = check_mouse_pos_params (&mouse_event, params_list, PARAMS_COUNT)) >= 0) {
                sort_servers (i);
                print_main_box (y, x);
                print_servers (y - 2, x - 4, OK);
            } else if ((i = check_mouse_pos_acts (y - 1, &mouse_event, acts_list, ACTS_COUNT)) >= 0) {
                acts_list [i].enabled = 1;
                acts_list [i].func (0, OK, y, x, acts_list [i].name);
                goto _key_loop_end;
            } else {
                /* TODO: Server Action Dialog */
            }
        }

        /* Filter Keys Handler */
        if (is_entering_filter) {
            if (filter_key_handler (ch)) {
                print_main_box (y, x);
                print_servers (y - 2, x - 4, OK);
            } goto _key_loop_end;
        } else if (ch == 'f') {
            is_entering_filter = 1;
            print_main_box (y, x);
            print_servers (y - 2, x - 4, OK);
            goto _key_loop_end;
        } else if (is_filtering && ch == KEY_DC) {
            drop_filter ();
            print_main_box (y, x);
            print_servers (y - 2, x - 4, OK);
        }

        if (check_move_key (ch)) {
            print_servers (y - 2, x - 4, ch);
        } else if (check_enter_key (ch)) {
            /* TODO: Handle This Type Of Keys */
        }

_key_loop_end:
        if (ch == ERR) {
            usleep (60000);
        } if (scan_status == scan_status_scanning) {
            sleep (1);
        } if (is_entering_filter && y >= MIN_LINES && x >= MIN_COLS) {
            curs_set (1);
            cursor_to_filter (x, 69);
        }
    }
}

void print_main_box (const int y, const int x) {
    register int i, x_pos;

    clear ();
    box (stdscr, 0, 0);
    for (i = 0, x_pos = 2; i < PARAMS_COUNT; x_pos += 1 + params_list [i].len, ++i)
        print_param (x_pos, &params_list [i]);

    for (i = 0, x_pos = 1; i < ACTS_COUNT; x_pos += 2 + acts_list [i].len, ++i)
        print_act (y - 1, x_pos, &acts_list [i]);

    print_filter (x, 69);
    refresh ();
}

void print_servers (const int y, const int x, const int ch) {
    int i;

    if (ch == OK) {
        goto _print_items;
    } if (ch == 'j' || ch == KEY_DOWN) {
        if (is_filtering) {
            if (current_filtered_serv_item + 1 < filtered_indexes_array_len) {
                if ((i = current_filtered_serv_item - filtered_items_shift) < y - 1) {
                    print_server (i + 1, x, filtered_indexes_array [current_filtered_serv_item++], 0);
                    print_server (i + 2, x, filtered_indexes_array [current_filtered_serv_item], 1);
                    print_current_item_num (y + 2, x + 4);
                    return;
                } else {
                    ++current_filtered_serv_item;
                }
            } else {
                return;
            }

            if (current_filtered_serv_item - filtered_items_shift == y) ++filtered_items_shift;
        } else {
            if (current_serv_item + 1 < serv_items_array_len) {
                if ((i = current_serv_item - items_shift) < y - 1) {
                    print_server (i + 1, x, current_serv_item++, 0);
                    print_server (i + 2, x, current_serv_item, 1);
                    print_current_item_num (y + 2, x + 4);
                    return;
                } else {
                    ++current_serv_item;
                }
            } else {
                return;
            }

            if (current_serv_item - items_shift == y) ++items_shift;
        }
    } if (ch == 'k' || ch == KEY_UP) {
        if (is_filtering) {
            if (current_filtered_serv_item) {
                if ((i = current_filtered_serv_item - filtered_items_shift) > 0) {
                    print_server (i + 1, x, filtered_indexes_array [current_filtered_serv_item--], 0);
                    print_server (i, x, filtered_indexes_array [current_filtered_serv_item], 1);
                    print_current_item_num (y + 2, x + 4);
                    return;
                } else {
                    --current_filtered_serv_item;
                }
            } else {
                return;
            }

            if (current_filtered_serv_item + 1 == filtered_items_shift) --filtered_items_shift;
        } else {
            if (current_serv_item) {
                if ((i = current_serv_item - items_shift) > 0) {
                    print_server (i + 1, x, current_serv_item--, 0);
                    print_server (i, x, current_serv_item, 1);
                    print_current_item_num (y + 2, x + 4);
                    return;
                } else {
                    --current_serv_item;
                }
            } else {
                return;
            }

            if (current_serv_item + 1 == items_shift) --items_shift;
        }
    }

_print_items:
    print_current_item_num (y + 2, x + 4);
    if (scan_status != scan_status_scanning && serv_items_array_len) {
        if (is_filtering) {
            for (i = 0; i < filtered_indexes_array_len && i < y; ++i) {
                if (filtered_items_shift + i == current_filtered_serv_item) {
                    print_server (i + 1, x, filtered_indexes_array [filtered_items_shift + i], 1);
                } else {
                    print_server (i + 1, x, filtered_indexes_array [filtered_items_shift + i], 0);
                }
            }
        } else {
            for (i = 0; items_shift + i < serv_items_array_len && i < y; ++i) {
                if (items_shift + i == current_serv_item) {
                    print_server (i + 1, x, items_shift + i, 1);
                } else {
                    print_server (i + 1, x, items_shift + i, 0);
                }
            }
        }
    }
}

void print_server (const int y, const int x, const int i, const char is_highlighted) {
    char *tmp_str;
    int x_pos = 2;

    if (is_highlighted)
        attron (A_REVERSE);

    tmp_str = malloc (24); /* Still Allocating For TODO Purposes */
    mvhline (y, 1, 0x20, x + 2);
    sprintf (tmp_str, "%d/%d", serv_items_array [i].online, serv_items_array [i].slots);

    mvaddnstr (y, 2, serv_items_array [i].ip, params_list [0].len);
    x_pos += params_list [0].len + 1;
    mvprintw (y, x_pos, "%hu", serv_items_array [i].port);
    x_pos += params_list [1].len + 1;
    mvaddnstr (y, x_pos, tmp_str, params_list [2].len);
    x_pos += params_list [2].len + 1;
    free (tmp_str);
    mvaddnstr (y, x_pos, serv_items_array [i].version, params_list [3].len);
    x_pos += params_list [3].len + 1;
    mvaddnstr (y, x_pos, serv_items_array [i].motd, x - x_pos - 3);
    x_pos += params_list [4].len + 1;

    if (is_highlighted)
        attroff (A_REVERSE);
}

void fin_curses (void) {
    endwin ();
}

void print_current_item_num (const int y, const int x) {
    int x_pos;
    char *tmp_str;

    if (is_filtering && filtered_indexes_array_len) {
        tmp_str = malloc (24);
        x_pos = sprintf (tmp_str, "%d/%d", current_filtered_serv_item + 1, filtered_indexes_array_len);
        mvhline (y - 1, x - 15, ACS_HLINE, 12);
        mvaddch (y - 1, x - x_pos - 3, ACS_LRCORNER);
        attron (COLOR_PAIR (3));
        addstr (tmp_str);
        attroff (COLOR_PAIR (3));
        addch (ACS_LLCORNER);
        free (tmp_str);
    } else if (!is_filtering && serv_items_array_len) {
        tmp_str = malloc (24);
        x_pos = sprintf (tmp_str, "%d/%d", current_serv_item + 1, serv_items_array_len);
        mvhline (y - 1, x - 15, ACS_HLINE, 12);
        mvaddch (y - 1, x - x_pos - 3, ACS_LRCORNER);
        attron (COLOR_PAIR (3));
        addstr (tmp_str);
        attroff (COLOR_PAIR (3));
        addch (ACS_LLCORNER);
        free (tmp_str);
    } else {
        mvhline (y - 1, x - 15, ACS_HLINE, 12);
        mvaddch (y - 1, x - 6, ACS_LRCORNER);
        attron (COLOR_PAIR (1));
        addstr ("0/0");
        attroff (COLOR_PAIR (1));
        addch (ACS_LLCORNER);
    }
}

