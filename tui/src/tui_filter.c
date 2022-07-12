/*****************************************************************************
 *
 *    tui/src/tui_filter.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include <stdlib.h>
#include <string.h>

#include "tui/tui_filter.h"
#include "tui/tui_utils.h"

#include "scan/scan.h"

int is_filtering,
    is_entering_filter;

char *filter_str;
int filter_str_len;

int *filtered_indexes_array,
    filtered_indexes_array_len,
    current_filtered_serv_item,
    filtered_items_shift;

void apply_serv_filter (const struct serv_item *, const int);
int compare_serv_with_filter (const struct serv_item *);

void print_filter (const int x, const int x_pos) {
    if (x_pos > x - 3) return;
    mvaddch (0, x_pos, ACS_URCORNER);
    attron (COLOR_PAIR (3));
    addch ('f');
    attroff (COLOR_PAIR (3));
    if (!is_entering_filter && !is_filtering) {
        addstr ("ilter");
    } else {
        addch (' ');
        if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
            if (filter_str_len > x - x_pos - 5) {
                printw ("%-*.*s", x - x_pos - 5, x - x_pos - 5, filter_str + (filter_str_len - (x - x_pos - 5)));
            } else {
                printw ("%-*.*s", x - x_pos - 5, x - x_pos - 5, filter_str ? filter_str : "");
            }
        } else {
            if (filter_str_len > MAX_FILTER_FIELD_LEN - 2) {
                printw ("%-*.*s", MAX_FILTER_FIELD_LEN - 2, MAX_FILTER_FIELD_LEN - 2, filter_str + (filter_str_len - (MAX_FILTER_FIELD_LEN - 2)));
            } else {
                printw ("%-*.*s", MAX_FILTER_FIELD_LEN - 2, MAX_FILTER_FIELD_LEN - 2, filter_str ? filter_str : "");
            }
        }
    } addch (ACS_ULCORNER);
}

/* TODO: Filter Entering String Size Limit */
int filter_key_handler (const int ch) {
    char *tmp_str;

    if (check_enter_key (ch)) {
        disable_entering_filter ();

        if (filter_str_len) {
            if (filtered_indexes_array) {
                free (filtered_indexes_array);
                filtered_indexes_array_len = 0;
            }

            current_filtered_serv_item = 0;
            is_filtering = 1;
            apply_serv_filter (serv_items_array, serv_items_array_len);
        } else {
            if (filtered_indexes_array) {
                free (filtered_indexes_array);
                filtered_indexes_array = 0;
                filtered_indexes_array_len = 0;
            } is_filtering = 0;
        }
    } else if (ch == KEY_BACKSPACE) {
        if (filter_str_len) {
            tmp_str = malloc (filter_str_len--);
            if (filter_str_len)
                memcpy (tmp_str, filter_str, filter_str_len);
            free (filter_str);
            filter_str = tmp_str;
            filter_str [filter_str_len] = 0;
        }
    } else {
        if (!filter_str_len) {
            if (filter_str)
                free (filter_str);
            filter_str = malloc (2);
            filter_str [filter_str_len++] = ch;
            filter_str [filter_str_len] = 0;
        } else {
            if (filter_str_len < MAX_FILTER_STR_LEN) {
                tmp_str = malloc (filter_str_len);
                memcpy (tmp_str, filter_str, filter_str_len);
                free (filter_str);
                filter_str = malloc (filter_str_len + 2);
                memcpy (filter_str, tmp_str, filter_str_len);
                filter_str [filter_str_len++] = ch;
                filter_str [filter_str_len] = 0;
                free (tmp_str);
            } else {
                return 0;
            }
        }
    } return 1;
}

void apply_serv_filter (const struct serv_item *serv_items_array, const int serv_items_array_len) {
    int *tmp_indexes_array, i;

    if (serv_items_array_len) {
        tmp_indexes_array = malloc (serv_items_array_len * sizeof (int));
        for (i = 0; i < serv_items_array_len; ++i) {
            if (compare_serv_with_filter (&serv_items_array [i])) {
                tmp_indexes_array [filtered_indexes_array_len++] = i;
            }
        }

        filtered_indexes_array = malloc (filtered_indexes_array_len * sizeof (int));
        for (i = 0; i < filtered_indexes_array_len; ++i) {
            filtered_indexes_array [i] = tmp_indexes_array [i];
        } free (tmp_indexes_array);
    }
}

int compare_serv_with_filter (const struct serv_item *serv_to_filter) {
    if (strstr (serv_to_filter->version, filter_str)) {
        return 1;
    } if (strstr (serv_to_filter->motd, filter_str)) {
        return 1;
    }

    return 0;
}

void cursor_to_filter (const int x, const int x_pos) {
    if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
        if (filter_str_len > x - x_pos - 6) {
            move (0, x - 3);
        } else {
            move (0, x_pos + 3 + filter_str_len);
        }
    } else {
        if (filter_str_len > MAX_FILTER_FIELD_LEN - 3) {
            move (0, x_pos + 3 + (MAX_FILTER_FIELD_LEN - 3));
        } else {
            move (0, x_pos + 3 + filter_str_len);
        }
    }
}

void disable_entering_filter (void) {
    is_entering_filter = 0;
    curs_set (0);
}

int check_mouse_pos_filter (const MEVENT *mouse_event) {
    return !mouse_event->y && mouse_event->x >= 70 && mouse_event->x < 76;
}

