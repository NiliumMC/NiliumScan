/*****************************************************************************
 *
 *    tui/src/tui_scan.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#include "tui/tui_utils.h"
#include "tui/tui_scan.h"
#include "scan/scan.h"

#define BOX_HEIGHT 7
#define BOX_WIDTH 64
#define BUTTONS_COUNT 2
#define TEXTFIELDS_COUNT 5
#define STATUS_IDLE_STR "IDLE"
#define STATUS_ERR_STR "ERROR"
#define STATUS_SCAN_STR "SCANNING"
#define STATUS_END_STR "SCANNED"

void start_scan_button (void);
int parse_ports (char *, unsigned short *, unsigned short *);
void print_scan_status (const int, const int);
int check_textfields (void);

struct tui_button scan_buttons [BUTTONS_COUNT] = { { 0, 0, COLOR_PAIR (3), "Start Scan", 10, 0, 4, 0, 6, 1 },
                                                   { 0, 0, COLOR_PAIR (1), "Cancel", 6, 1, 4, 1, 0, 2 } };

struct tui_textfield scan_textfields [TEXTFIELDS_COUNT] = { { 0, 0, COLOR_PAIR (3), "IP", 2, 28, 50, 0, 0, 2, 2, 4, 1, 3 },
                                                            { 0, 0, COLOR_PAIR (3), "Port Range", 10, 11, 11, 0, 0, 3, 3, 6, 2, 4 },
                                                            { 0, 0, COLOR_PAIR (3), "MC Protocol", 11, 11, 11, 0, 0, 4, 2, 0, 3, 5 },
                                                            { 0, 0, COLOR_PAIR (3), "Scan Threads", 12, 2, 2, 0, 0, 5, 3, 1, 4, 6 },
                                                            { 0, 0, COLOR_PAIR (3), "Timeout", 7, 1, 1, 0, 0, 6, 3, 1, 5, 0 } };

pthread_t *scan_threads;
int num_threads;

struct scan_args sargs;

int scan_current,
    scan_status;

int act_scan (const MEVENT *mouse_event, int ch, const int y, const int x, const char *name) {
    static int y_pos, x_pos;
    int tmp_current;

    if (scan_status == scan_status_scanning && !is_scanning) {
        scan_status = scan_status_end;
        copy_serv_list_to_array ();
        free (scan_threads);
    } if (ch == OK) {
        scan_current = 0;
    }

    if (check_current_textfield ()) {
        if (mouse_event) {
            goto _check_mouse;
        } if (ch == KEY_ENTER || ch == '\n') {
            ch = KEY_STAB;
        /* TODO: RegEX buttons check */
        } if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == KEY_BACKSPACE) {
            enter_to_textfield (ch, scan_current, scan_textfields, TEXTFIELDS_COUNT);
        } if (ch != KEY_UP && ch != KEY_DOWN && ch != KEY_LEFT && ch != KEY_RIGHT && ch != KEY_STAB && ch != '\t' && ch != KEY_BTAB) {
            goto _calc_size;
        }
    }

    if (ch == 'q') {
        curs_set (0);
        return 0;
    } if (ch == 'y') {
        scan_current = 0;
        scan_status = scan_status_idle;
    } if (ch == 'n') {
        scan_current = 1;
        scan_status = scan_status_idle;
    }

    if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT ||
        ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l' ||
        ch == '\t' || ch == KEY_STAB || ch == KEY_BTAB) {
        scan_current = find_next_item (ch, 0, &scan_current, scan_buttons, BUTTONS_COUNT, scan_textfields, TEXTFIELDS_COUNT);
        scan_status = scan_status_idle;
    }

    if (ch == '\n' || ch == KEY_ENTER) {
        if (scan_current == 0) {
            start_scan_button ();
        } else if (scan_current == 1) {
            return 0;
        }
    }

_check_mouse:
    if (mouse_event && (mouse_event->bstate & BUTTON1_CLICKED || mouse_event->bstate & BUTTON3_CLICKED)) {
        tmp_current = find_next_item (ch, mouse_event, &scan_current, scan_buttons, BUTTONS_COUNT, scan_textfields, TEXTFIELDS_COUNT);
        scan_status = scan_status_idle;

        if (scan_current == tmp_current && tmp_current == 0) {
            start_scan_button ();
        } if (scan_current == tmp_current && tmp_current == 1) {
            curs_set (0);
            return 0;
        }
    } if (mouse_event && (mouse_event->bstate & BUTTON1_DOUBLE_CLICKED || mouse_event->bstate & BUTTON3_DOUBLE_CLICKED)) {
        find_next_item (ch, mouse_event, &scan_current, scan_buttons, BUTTONS_COUNT, scan_textfields, TEXTFIELDS_COUNT);
        scan_status = scan_status_idle;

        if (scan_current == 0) {
            start_scan_button ();
        } if (scan_current == 1) {
            curs_set (0);
            return 0;
        }
    }

_calc_size:
    y_pos = (y / 2) - (BOX_HEIGHT / 2);
    x_pos = (x / 2) - (BOX_WIDTH / 2);
    scan_buttons [0].y_pos = y_pos + 6;
    scan_buttons [0].x_pos = x_pos + 2;
    scan_buttons [1].y_pos = y_pos + 6;
    scan_buttons [1].x_pos = x_pos + 15;
    scan_textfields [0].y_pos = y_pos + 2;
    scan_textfields [0].x_pos = x_pos + 2;
    scan_textfields [1].y_pos = y_pos + 2;
    scan_textfields [1].x_pos = x_pos + 37;
    scan_textfields [2].y_pos = y_pos + 4;
    scan_textfields [2].x_pos = x_pos + 2;
    scan_textfields [3].y_pos = y_pos + 4;
    scan_textfields [3].x_pos = x_pos + 30;
    scan_textfields [4].y_pos = y_pos + 4;
    scan_textfields [4].x_pos = x_pos + 50;

    print_clear_win_at (y_pos, x_pos, BOX_HEIGHT, BOX_WIDTH, name);
    print_tui_buttons (scan_current, scan_buttons, BUTTONS_COUNT);
    print_tui_textfields (scan_current, scan_textfields, TEXTFIELDS_COUNT);
    print_scan_status (y_pos + 6, x_pos + 26);

    if (check_current_textfield ()) {
        cursor_to_textfield (scan_current, scan_textfields, TEXTFIELDS_COUNT);
    } else {
        curs_set (0);
    }

    refresh ();
    return 1;
}

int check_current_textfield (void) {
    int i;

    for (i = 0; i < TEXTFIELDS_COUNT; ++i)
        if (scan_current == scan_textfields [i].current_id)
            return 1;

    return 0;
}

void start_scan_button (void) {
    unsigned short port_fst, port_lst, num_threads;
    unsigned int i;

    if (check_textfields ()) {
        if (parse_ports (scan_textfields [1].buf, &port_fst, &port_lst) && (num_threads = atoi (scan_textfields [3].buf))) {
            scan_threads = malloc (sizeof (pthread_t) * num_threads);
            char *tmp_ip_buf = malloc (scan_textfields [0].buf_size);
            strcpy (tmp_ip_buf, scan_textfields [0].buf);

            sargs.ip = tmp_ip_buf;
            sargs.ip_len = scan_textfields [0].buf_size;
            sargs.protocol = atoi (scan_textfields [2].buf);
            sargs.timeout = atoi (scan_textfields [4].buf);
            sargs.port_fst = port_fst;
            sargs.port_lst = port_lst;

            is_scanning = 1;
            scan_status = scan_status_scanning;
            if (serv_items_array) {
                for (i = 0; i < serv_items_array_len; ++i) {
                    free_serv_item (&serv_items_array [i]);
                } free (serv_items_array);
                serv_items_array = 0;
            } for (i = 0; i < num_threads; ++i) {
                pthread_create (&scan_threads [i], 0, start_scan, (void *) &sargs);
                pthread_detach (scan_threads [i]);
            }
        } else {
            scan_status = scan_status_error;
        }
    } else {
        scan_status = scan_status_error;
    }
}

int parse_ports (char *ports_str, unsigned short *fst, unsigned short *lst) {
    char *delim;

    if (!(delim = strchr (ports_str, '-')))
        return 0;

    if (!(*fst = atoi (ports_str)) || !(*lst = atoi (delim + 1)))
        return 0;

    if (*fst > *lst)
        return 0;

    return 1;
}

void print_scan_status (const int y, const int x) {
    mvaddch (y, x, '[');
    addstr ("Status: ");
    if (scan_status == scan_status_idle) {
        attron (COLOR_PAIR (3));
        addstr (STATUS_IDLE_STR);
        attroff (COLOR_PAIR (3));
    } else if (scan_status == scan_status_error) {
        attron (COLOR_PAIR (1));
        addstr (STATUS_ERR_STR);
        attroff (COLOR_PAIR (1));
    } else if (scan_status == scan_status_scanning) {
        /* TODO: Print Scanning Percentage */
        attron (COLOR_PAIR (3));
        addstr (STATUS_SCAN_STR);
        attroff (COLOR_PAIR (3));
    } else if (scan_status == scan_status_end) {
        attron (COLOR_PAIR (2));
        addstr (STATUS_END_STR);
        attroff (COLOR_PAIR (2));
    } addch (']');
}

int check_textfields (void) {
    int i;

    for (i = 0; i < TEXTFIELDS_COUNT; ++i)
        if (!scan_textfields [i].buf || !scan_textfields [i].buf_size)
            return 0;

    return 1;
}

