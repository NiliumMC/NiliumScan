/*****************************************************************************
 *
 *    tui/src/tui_utils.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

#include <string.h>
#include <stdlib.h>

#include "tui/tui_filter.h"
#include "tui/tui_utils.h"

#include "scan/scan.h"

void print_tui_button (const struct tui_button *, const int); /* Print One TUI Button */
void print_tui_textfield (const struct tui_textfield *, const int); /* Print One TUI TextField */
int check_mouse_pos_button (const struct tui_button *, const int, const int); /* Check If The Pressed Mouse Key Is On TUI Button */
int button_get_next (const struct tui_button *, int); /* Find Next TUI Item Index From TUI Button */
int check_mouse_pos_textfield (const struct tui_textfield *, const int, const int); /* Check If The Pressed Mouse Key Is On TUI TextField */
int textfield_get_next (const struct tui_textfield *, int); /* Find Next TUI Item Index From TUI TextField */

void print_min_size (const int min_lines, const int min_cols, const int y, const int x) {
    int y_pos = (y >> 1) - 2, x_pos = (x >> 1) - 16;

    clear ();
    curs_set (0);
    mvprintw (y_pos++, x_pos, "Your terminal is so small!");
    mvprintw (y_pos++, x_pos, "      Lines: %d/", min_lines);
    if (y >= min_lines) {
        attron (COLOR_PAIR (2));
        printw ("%d ", y);
        attroff (COLOR_PAIR (2));
    } else {
        attron (COLOR_PAIR (1));
        printw ("%d ", y);
        attroff (COLOR_PAIR (1));
    }

    mvprintw (y_pos, x_pos, "      Cols: %d/", min_cols);
    if (x >= min_cols) {
        attron (COLOR_PAIR (2));
        printw ("%d", x);
        attroff (COLOR_PAIR (2));
    } else {
        attron (COLOR_PAIR (1));
        printw ("%d", x);
        attroff (COLOR_PAIR (1));
    }
}

void print_clear_win_at (const int y_pos, const int x_pos, const int height, const int width, const char *name) {
    int i, j;

    for (i = 0; i < height; ++i)
        for (j = 0; j < width; ++j)
            mvaddch (y_pos + i, x_pos + j, ' ');

    mvaddch (y_pos, x_pos, ACS_ULCORNER);
    mvaddch (y_pos, x_pos + width - 1, ACS_URCORNER);
    mvaddch (y_pos + height - 1, x_pos, ACS_LLCORNER);
    mvaddch (y_pos + height - 1, x_pos + width - 1, ACS_LRCORNER);
    mvhline (y_pos, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos, ACS_VLINE, height - 2);
    mvhline (y_pos + height - 1, x_pos + 1, ACS_HLINE, width - 2);
    mvvline (y_pos + 1, x_pos + width - 1, ACS_VLINE, height - 2);

    mvaddch (y_pos, x_pos + 2, ACS_URCORNER);
    attron (COLOR_PAIR (3));
    addstr (name);
    attroff (COLOR_PAIR (3));
    addch (ACS_ULCORNER);
}

void print_tui_button (const struct tui_button *button, const int attrs) {
    mvaddch (button->y_pos, button->x_pos, '[');
    attron (attrs);
    addstr (button->name);
    attroff (attrs);
    addch (']');
}

void print_tui_textfield (const struct tui_textfield *textfield, const int attrs) {
    mvprintw (textfield->y_pos, textfield->x_pos, "%s: [", textfield->name);
    attron (attrs);
    if (textfield->buf) {
        if (textfield->buf_size >= textfield->len_buf) {
            addstr (textfield->buf + (textfield->buf_size - textfield->len_buf));
        } else {
            addstr (textfield->buf);
        }
    }
    attroff (attrs);
    mvaddch (textfield->y_pos, textfield->x_pos + textfield->len_name + textfield->len_buf + 3, ']');
}

void print_tui_buttons (const int current_item, const struct tui_button *buttons, const int num_buttons) {
    int i;

    for (i = 0; i < num_buttons; ++i) {
        if (current_item == buttons [i].current_id) {
            print_tui_button (&buttons [i], buttons [i].attrs | (int) A_REVERSE | (int) A_BOLD);
        } else {
            print_tui_button (&buttons [i], buttons [i].attrs);
        }
    }
}

void print_tui_textfields (const int current_item, const struct tui_textfield *textfields, const int num_textfields) {
    int i;

    for (i = 0; i < num_textfields; ++i) {
        if (current_item == textfields [i].current_id) {
            print_tui_textfield (&textfields [i], textfields [i].attrs | (int) A_BOLD);
        } else {
            print_tui_textfield (&textfields [i], textfields [i].attrs);
        }
    }
}

int check_mouse_pos_button (const struct tui_button *button, const int y_pos, const int x_pos) {
    return y_pos == button->y_pos && x_pos >= button->x_pos && x_pos <= button->x_pos + button->len + 1;
}

int button_get_next (const struct tui_button *button, int direction) {
    if (direction == direct_up) {
        return button->up_id;
    } else if (direction == direct_down) {
        return button->down_id;
    } else if (direction == direct_left) {
        return button->left_id;
    } else if (direction == direct_right) {
        return button->right_id;
    } /* `else {}` is optimized */
    return -1;
}

int check_mouse_pos_textfield (const struct tui_textfield *textfield, const int y_pos, const int x_pos) {
    return y_pos == textfield->y_pos && x_pos >= textfield->x_pos && x_pos <= textfield->x_pos + textfield->len_name + textfield->len_buf + 3;
}

int textfield_get_next (const struct tui_textfield *textfield, int direction) {
    if (direction == direct_up) {
        return textfield->up_id;
    } else if (direction == direct_down) {
        return textfield->down_id;
    } else if (direction == direct_left) {
        return textfield->left_id;
    } else if (direction == direct_right) {
        return textfield->right_id;
    }  /* `else {}` is optimized */
    return -1;
}

int find_next_item (const int ch, const MEVENT *mouse_event, int *current_item, const struct tui_button *buttons, const int num_buttons, const struct tui_textfield *textfields, const int num_textfields) {
    int i, tmp_current;

    /* Buttons Enum */
    if (buttons && num_buttons) {
        for (i = 0; i < num_buttons; ++i) {
            if (!mouse_event && buttons [i].current_id == *current_item) {
                if (ch == KEY_UP || ch == 'k') {
                    return (*current_item = button_get_next (&buttons [i], direct_up));
                } else if (ch == KEY_DOWN || ch == 'j') {
                    return (*current_item = button_get_next (&buttons [i], direct_down));
                } else if (ch == KEY_LEFT || ch == 'h' || ch == KEY_BTAB) {
                    return (*current_item = button_get_next (&buttons [i], direct_left));
                } else if (ch == KEY_RIGHT || ch == 'l' || ch == '\t' || ch == KEY_STAB) {
                    return (*current_item = button_get_next (&buttons [i], direct_right));
                } else {
                    return (*current_item = buttons [i].current_id);
                }
            } if (mouse_event) {
                if (check_mouse_pos_button (&buttons [i], mouse_event->y, mouse_event->x)) {
                    tmp_current = *current_item;
                    *current_item = buttons [i].current_id;
                    return tmp_current;
                }
            }
        }
    }

    /* Textfields Enum */
    if (textfields && num_textfields) {
        for (i = 0; i < num_textfields; ++i) {
            if (!mouse_event && textfields [i].current_id == *current_item) {
                if (ch == KEY_UP || ch == 'k') {
                    return (*current_item = textfield_get_next (&textfields [i], direct_up));
                } else if (ch == KEY_DOWN || ch == 'j') {
                    return (*current_item = textfield_get_next (&textfields [i], direct_down));
                } else if (ch == KEY_LEFT || ch == 'h' || ch == KEY_BTAB) {
                    return (*current_item = textfield_get_next (&textfields [i], direct_left));
                } else if (ch == KEY_RIGHT || ch == 'l' || ch == '\t' || ch == KEY_STAB) {
                    return (*current_item = textfield_get_next (&textfields [i], direct_right));
                } else {
                    return (*current_item = textfields [i].current_id);
                }
             } if (mouse_event) {
                if (check_mouse_pos_textfield (&textfields [i], mouse_event->y, mouse_event->x)) {
                    return (*current_item = textfields [i].current_id);
                }
            }
        }
    }

    return -1;
}

void enter_to_textfield (const int ch, const int current_item, struct tui_textfield *textfields, const int num_textfields) {
    int i;

    for (i = 0; i < num_textfields; ++i) {
        if (textfields [i].current_id == current_item) {
            if (ch == KEY_BACKSPACE) {
                if (textfields [i].buf && textfields [i].buf_size > 0) {
                    textfields [i].buf [--textfields [i].buf_size] = 0;
                } return;
            } if (!textfields [i].buf) {
                textfields [i].buf = malloc ((unsigned int) (textfields [i].max_buf_len + 1));
                memset (textfields [i].buf, 0, (unsigned int) (textfields [i].max_buf_len + 1));
            } if (textfields [i].buf_size < textfields [i].max_buf_len) {
                textfields [i].buf [textfields [i].buf_size++] = (char) ch;
            }
        }
    }
}

void cursor_to_textfield (const int current_item, const struct tui_textfield *textfields, const int num_textfields) {
    int i = 0;

    for (i = 0; i < num_textfields; ++i) {
        if (textfields [i].current_id == current_item) {
            curs_set (1);
            if (textfields [i].buf && textfields [i].buf_size >= textfields [i].len_buf) {
                move (textfields [i].y_pos, textfields [i].x_pos + textfields [i].len_name + 3 + textfields [i].len_buf - 1);
            } else {
                move (textfields [i].y_pos, textfields [i].x_pos + textfields [i].len_name + 3 + textfields [i].buf_size);
            }
        }
    }
}

int check_entering_field_key (const int ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == KEY_BACKSPACE || ch == ' ';
}

int check_move_key (const int ch) {
    return ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT ||
           ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l' ||
           ch == '\t' || ch == KEY_STAB || ch == KEY_BTAB;
}

int check_enter_key (const int ch) {
    return ch == '\n' || ch == KEY_ENTER;
}

int check_mouse_click (const MEVENT *mouse_event) {
    return mouse_event && (mouse_event->bstate & BUTTON1_CLICKED || mouse_event->bstate & BUTTON3_CLICKED);
}

int check_mouse_double_click (const MEVENT *mouse_event) {
    return mouse_event && (mouse_event->bstate & BUTTON1_DOUBLE_CLICKED || mouse_event->bstate & BUTTON3_DOUBLE_CLICKED);
}

int check_mouse_pos_box (const int y_pos, const int x_pos, const int y, const int x, const int height, const int width) {
    return y_pos >= y && y_pos < y + height && x_pos >= x && x_pos < x + width;
}

int check_mouse_pos_serv_list (const int y, const int x, const int items_shift, const MEVENT *mouse_event) {
    if (is_filtering) {
        if (filtered_items_shift + mouse_event->y - 1 < filtered_indexes_array_len && mouse_event->y > 0 && mouse_event->y <= y && mouse_event->x > 0 && mouse_event->x <= x) {
            return filtered_items_shift + mouse_event->y - 1;
        }
    } else {
        if (items_shift + mouse_event->y - 1 < serv_items_array_len && mouse_event->y > 0 && mouse_event->y <= y && mouse_event->x > 0 && mouse_event->x <= x) {
            return items_shift + mouse_event->y - 1;
        }
    }

    return -1;
}

void sort_servers (const int param) {
    if (param == 0) {
        if (is_filtering) {
            qsort (filtered_indexes_array, (unsigned int) filtered_indexes_array_len, sizeof (int), compare_serv_ip);
        } else {
            qsort (serv_items_array, (unsigned int) serv_items_array_len, sizeof (struct serv_item), compare_serv_ip);
        }
    } else if (param == 1) {
        if (is_filtering) {
            qsort (filtered_indexes_array, (unsigned int) filtered_indexes_array_len, sizeof (int), compare_serv_ports);
        } else {
            qsort (serv_items_array, (unsigned int) serv_items_array_len, sizeof (struct serv_item), compare_serv_ports);
        }
    } else if (param == 2) {
        if (is_filtering) {
            qsort (filtered_indexes_array, (unsigned int) filtered_indexes_array_len, sizeof (int), compare_serv_online);
        } else {
            qsort (serv_items_array, (unsigned int) serv_items_array_len, sizeof (struct serv_item), compare_serv_online);
        }
    } else if (param == 3) {
        if (is_filtering) {
            qsort (filtered_indexes_array, (unsigned int) filtered_indexes_array_len, sizeof (int), compare_serv_version);
        } else {
            qsort (serv_items_array, (unsigned int) serv_items_array_len, sizeof (struct serv_item), compare_serv_version);
        }
    } else if (param == 4) {
        if (is_filtering) {
            qsort (filtered_indexes_array, (unsigned int) filtered_indexes_array_len, sizeof (int), compare_serv_motd);
        } else {
            qsort (serv_items_array, (unsigned int) serv_items_array_len, sizeof (struct serv_item), compare_serv_motd);
        }
    }
}

int compare_serv_ip (const void *serv_item_fst, const void *serv_item_snd) {
    register int i;

    if (is_filtering) {
        for (i = 0; serv_items_array [*((const int *) serv_item_fst)].ip [i] && serv_items_array [*((const int *) serv_item_snd)].ip [i]; ++i) {
            if (serv_items_array [*((const int *) serv_item_fst)].ip [i] > serv_items_array [*((const int *) serv_item_snd)].ip [i] ||
                (serv_items_array [*((const int *) serv_item_fst)].ip [i + 1] && !serv_items_array [*((const int *) serv_item_snd)].ip [i + 1])) {
                return -1;
            } else if (serv_items_array [*((const int *) serv_item_fst)].ip [i] < serv_items_array [*((const int *) serv_item_snd)].ip [i] ||
                       (!serv_items_array [*((const int *) serv_item_fst)].ip [i + 1] && serv_items_array [*((const int *) serv_item_snd)].ip [i + 1])) {
                return 1;
            }
        }
    } else {
        for (i = 0; ((const struct serv_item *) serv_item_fst)->ip [i] && ((const struct serv_item *) serv_item_snd)->ip [i]; ++i) {
            if (((const struct serv_item *) serv_item_fst)->ip [i] > ((const struct serv_item *) serv_item_snd)->ip [i] ||
                (((const struct serv_item *) serv_item_fst)->ip [i + 1] && !((const struct serv_item *) serv_item_snd)->ip [i + 1])) {
                return -1;
            } else if (((const struct serv_item *) serv_item_fst)->ip [i] < ((const struct serv_item *) serv_item_snd)->ip [i] ||
                       (!((const struct serv_item *) serv_item_fst)->ip [i + 1] && ((const struct serv_item *) serv_item_snd)->ip [i + 1])) {
                return 1;
            }
        }
    } return 0;
}

int compare_serv_ports (const void *serv_item_fst, const void *serv_item_snd) {
    if (is_filtering) {
        return (serv_items_array [*((const int *) serv_item_fst)].port - serv_items_array [*((const int *) serv_item_snd)].port);
    } else {
        return (((const struct serv_item *) serv_item_fst)->port - ((const struct serv_item *) serv_item_snd)->port);
    }
}

int compare_serv_online (const void *serv_item_fst, const void *serv_item_snd) {
    if (is_filtering) {
        return (serv_items_array [*((const int *) serv_item_snd)].online - serv_items_array [*((const int *) serv_item_fst)].online);
    } else {
        return (((const struct serv_item *) serv_item_snd)->online - ((const struct serv_item *) serv_item_fst)->online);
    }
}

int compare_serv_version (const void *serv_item_fst, const void *serv_item_snd) {
    register int i;

    if (is_filtering) {
        for (i = 0; serv_items_array [*((const int *) serv_item_fst)].version [i] && serv_items_array [*((const int *) serv_item_snd)].version [i]; ++i) {
            if (serv_items_array [*((const int *) serv_item_fst)].version [i] > serv_items_array [*((const int *) serv_item_snd)].version [i] ||
                (serv_items_array [*((const int *) serv_item_fst)].version [i + 1] && !serv_items_array [*((const int *) serv_item_snd)].version [i + 1])) {
                return -1;
            } else if (serv_items_array [*((const int *) serv_item_fst)].version [i] < serv_items_array [*((const int *) serv_item_snd)].version [i] ||
                       (!serv_items_array [*((const int *) serv_item_fst)].version [i + 1] && serv_items_array [*((const int *) serv_item_snd)].version [i + 1])) {
                return 1;
            }
        }
    } else {
        for (i = 0; ((const struct serv_item *) serv_item_fst)->version [i] && ((const struct serv_item *) serv_item_snd)->version [i]; ++i) {
            if (((const struct serv_item *) serv_item_fst)->version [i] > ((const struct serv_item *) serv_item_snd)->version [i] ||
                (((const struct serv_item *) serv_item_fst)->version [i + 1] && !((const struct serv_item *) serv_item_snd)->version [i + 1])) {
                return -1;
            } else if (((const struct serv_item *) serv_item_fst)->version [i] < ((const struct serv_item *) serv_item_snd)->version [i] ||
                       (!((const struct serv_item *) serv_item_fst)->version [i + 1] && ((const struct serv_item *) serv_item_snd)->version [i + 1])) {
                return 1;
            }
        }
    } return 0;
}

int compare_serv_motd (const void *serv_item_fst, const void *serv_item_snd) {
    register int i;

    if (is_filtering) {
        for (i = 0; serv_items_array [*((const int *) serv_item_fst)].motd [i] && serv_items_array [*((const int *) serv_item_snd)].motd [i]; ++i) {
            if (serv_items_array [*((const int *) serv_item_fst)].motd [i] > serv_items_array [*((const int *) serv_item_snd)].motd [i] ||
                (serv_items_array [*((const int *) serv_item_fst)].motd [i + 1] && !serv_items_array [*((const int *) serv_item_snd)].motd [i + 1])) {
                return -1;
            } else if (serv_items_array [*((const int *) serv_item_fst)].motd [i] < serv_items_array [*((const int *) serv_item_snd)].motd [i] ||
                       (!serv_items_array [*((const int *) serv_item_fst)].motd [i + 1] && serv_items_array [*((const int *) serv_item_snd)].motd [i + 1])) {
                return 1;
            }
        }
    } else {
        for (i = 0; ((const struct serv_item *) serv_item_fst)->motd [i] && ((const struct serv_item *) serv_item_snd)->motd [i]; ++i) {
            if (((const struct serv_item *) serv_item_fst)->motd [i] > ((const struct serv_item *) serv_item_snd)->motd [i] ||
                (((const struct serv_item *) serv_item_fst)->motd [i + 1] && !((const struct serv_item *) serv_item_snd)->motd [i + 1])) {
                return -1;
            } else if (((const struct serv_item *) serv_item_fst)->motd [i] < ((const struct serv_item *) serv_item_snd)->motd [i] ||
                       (!((const struct serv_item *) serv_item_fst)->motd [i + 1] && ((const struct serv_item *) serv_item_snd)->motd [i + 1])) {
                return 1;
            }
        }
    } return 0;
}

