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

char *filter_str; /* Servers List Filter String */
int filter_str_len; /* Servers List Filter String Lenght */

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
        addch (ACS_ULCORNER);
    } else {
        addch (' ');
        if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
            if (filter_str_len >= x - x_pos - (is_entering_filter ? 10 : 8)) {
                printw ("%.*s", x - x_pos - (is_entering_filter ? 10 : 8), filter_str + (filter_str_len - (x - x_pos - (is_entering_filter ? 10 : 8))));
                addch (' ');

                if (is_entering_filter) {
                    attron (COLOR_PAIR (2) | A_REVERSE);
                    addstr ("OK");
                    attroff (COLOR_PAIR (2) | A_REVERSE);
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                } else {
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                }

                addch (ACS_ULCORNER);
            } else {
                printw ("%.*s", x - x_pos - 6, filter_str ? filter_str : "");

                if (is_entering_filter) {
                    addstr ("  ");
                    if (filter_str_len) {
                        attron (COLOR_PAIR (2) | A_REVERSE);
                        addstr ("OK");
                        attroff (COLOR_PAIR (2) | A_REVERSE);
                        attron (COLOR_PAIR (1) | A_REVERSE);
                        addstr ("DL");
                        attroff (COLOR_PAIR (1) | A_REVERSE);
                    } else {
                        attron (COLOR_PAIR (1) | A_REVERSE);
                        addstr ("DL");
                        attroff (COLOR_PAIR (1) | A_REVERSE);
                    }
                } else {
                    addch (' ');
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                }

                addch (ACS_ULCORNER);
            }
        } else {
            if (filter_str_len >= MAX_FILTER_FIELD_LEN - (is_entering_filter ? 7 : 3)) {
                printw ("%.*s", MAX_FILTER_FIELD_LEN - (is_entering_filter ? 7 : 3), filter_str + (filter_str_len - (MAX_FILTER_FIELD_LEN - (is_entering_filter ? 7 : 3))));
                addch (' ');

                if (is_entering_filter) {
                    attron (COLOR_PAIR (2) | A_REVERSE);
                    addstr ("OK");
                    attroff (COLOR_PAIR (2) | A_REVERSE);
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                } else {
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                }

                addch (ACS_ULCORNER);
            } else {
                printw ("%.*s", MAX_FILTER_FIELD_LEN - (is_entering_filter ? 7 : 3), filter_str ? filter_str : "");

                if (is_entering_filter) {
                    addstr ("  ");
                    if (filter_str_len) {
                        attron (COLOR_PAIR (2) | A_REVERSE);
                        addstr ("OK");
                        attroff (COLOR_PAIR (2) | A_REVERSE);
                        attron (COLOR_PAIR (1) | A_REVERSE);
                        addstr ("DL");
                        attroff (COLOR_PAIR (1) | A_REVERSE);
                    } else {
                        attron (COLOR_PAIR (1) | A_REVERSE);
                        addstr ("DL");
                        attroff (COLOR_PAIR (1) | A_REVERSE);
                    }
                } else {
                    addch (' ');
                    attron (COLOR_PAIR (1) | A_REVERSE);
                    addstr ("DL");
                    attroff (COLOR_PAIR (1) | A_REVERSE);
                }

                addch (ACS_ULCORNER);
            }
        }
    }
}

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
            /* is_filter_applied = 1; */
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
            tmp_str = malloc ((unsigned int) filter_str_len--);
            if (filter_str_len)
                memcpy (tmp_str, filter_str, (unsigned int) filter_str_len);
            free (filter_str);
            filter_str = tmp_str;
            filter_str [filter_str_len] = 0;
        }
    } else if (check_entering_field_key (ch)) {
        if (!filter_str_len) {
            if (filter_str)
                free (filter_str);
            filter_str = malloc (2);
            filter_str [filter_str_len++] = (char) ch;
            filter_str [filter_str_len] = 0;
        } else {
            if (filter_str_len < MAX_FILTER_STR_LEN) {
                tmp_str = malloc ((unsigned int) filter_str_len);
                memcpy (tmp_str, filter_str, (unsigned int) filter_str_len);
                free (filter_str);
                filter_str = malloc ((unsigned int) (filter_str_len + 2));
                memcpy (filter_str, tmp_str, (unsigned int) filter_str_len);
                filter_str [filter_str_len++] = (char) ch;
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
        tmp_indexes_array = malloc ((unsigned int) serv_items_array_len * sizeof (int));
        for (i = 0; i < serv_items_array_len; ++i) {
            if (compare_serv_with_filter (&serv_items_array [i])) {
                tmp_indexes_array [filtered_indexes_array_len++] = i;
            }
        }

        if (filtered_indexes_array_len) {
            filtered_indexes_array = malloc ((unsigned int) filtered_indexes_array_len * sizeof (int));
            for (i = 0; i < filtered_indexes_array_len; ++i) {
                filtered_indexes_array [i] = tmp_indexes_array [i];
            }
        } free (tmp_indexes_array);
    }
}

/* TODO: Configure Using This Version Of Filter */
int compare_serv_with_filter (const struct serv_item *serv_to_filter) {
    char *tmp_filter_str, *filter_str_tkn;

    tmp_filter_str = malloc ((unsigned int) (filter_str_len + 1));
    strcpy (tmp_filter_str, filter_str);

    filter_str_tkn = strtok (tmp_filter_str, " ");
    while (filter_str_tkn) {
        if (strstr (serv_to_filter->version, filter_str_tkn)) {
            free (tmp_filter_str);
            return 1;
        } if (strstr (serv_to_filter->motd, filter_str_tkn)) {
            free (tmp_filter_str);
            return 1;
        } filter_str_tkn = strtok (0, " ");
    }

    free (tmp_filter_str);
    return 0;
}

void cursor_to_filter (const int x, const int x_pos) {
    if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
        if (filter_str_len > x - x_pos - 11) {
            move (0, x - 8);
        } else {
            move (0, x_pos + 3 + filter_str_len);
        }
    } else {
        if (filter_str_len >= MAX_FILTER_FIELD_LEN - 7) {
            move (0, x_pos + 3 + (MAX_FILTER_FIELD_LEN - 8));
        } else {
            move (0, x_pos + 3 + filter_str_len);
        }
    }
}

void disable_entering_filter (void) {
    is_entering_filter = 0;
    curs_set (0);
}

void drop_filter (void) {
    free (filter_str);
    filter_str = malloc (1);
    filter_str [0] = 0;
    filter_str_len = 0;
    is_filtering = 0;
}

int check_mouse_pos_filter (const MEVENT *mouse_event) {
    return !mouse_event->y && mouse_event->x >= 70 && mouse_event->x < 76;
}

int check_mouse_pos_filter_ok_button (const int x, const int x_pos, const MEVENT *mouse_event) {
    register int i;

    if (!mouse_event->y) {
        if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
            if (filter_str_len > x - x_pos - 10) {
                return mouse_event->x >= x - 6 && mouse_event->x <= x - 5;
            } else {
                i = x_pos + (filter_str_len ? 5 : 6) + filter_str_len;
                return mouse_event->x >= i && mouse_event->x <= i + 1;
            }
        } else {
            if (filter_str_len >= MAX_FILTER_FIELD_LEN - 6) {
                i = x_pos + 3 + (MAX_FILTER_FIELD_LEN - 6);
                return mouse_event->x >= i && mouse_event->x <= i + 1;
            } else if (filter_str_len) {
                i = x_pos + 5 + filter_str_len;
                return mouse_event->x >= i && mouse_event->x <= i + 1;
            }
        }
    }

    return 0;
}

int check_mouse_pos_filter_dl_button (const int x, const int x_pos, const MEVENT *mouse_event) {
    register int i;

    if (!mouse_event->y) {
        if (x_pos + MAX_FILTER_FIELD_LEN >= x - 2) {
            if (filter_str_len > x - x_pos - (is_entering_filter ? 6 : 8)) {
                return mouse_event->x >= x - 4 && mouse_event->x <= x - 3;
            } else {
                i = x_pos + (filter_str_len ? 4 : 5) + filter_str_len;
                return mouse_event->x >= i && mouse_event->x <= i + 1;
            }
        } else {
            if (filter_str_len >= MAX_FILTER_FIELD_LEN - (is_entering_filter ? 7 : 5)) {
                i = x_pos + (is_entering_filter ? 5 : 7) + (MAX_FILTER_FIELD_LEN - 6);
                return mouse_event->x >= i && mouse_event->x <= i + 1;
            } else {
                if (is_entering_filter) {
                    i = x_pos + (filter_str_len ? 7 : 5) + filter_str_len;
                } else {
                    if (filter_str_len) {
                        i = x_pos + 4 + filter_str_len;
                    } else {
                        return 0;
                    }
                }

                return mouse_event->x >= i && mouse_event->x <= i + 1;
            }
        }
    }

    return 0;
}

