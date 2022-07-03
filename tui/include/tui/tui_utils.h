/*****************************************************************************
 *
 *    tui/include/tui/tui_utils.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <ncurses.h>

enum tui_direct {
    direct_up,
    direct_down,
    direct_left,
    direct_right
};

struct tui_button {
    int y_pos, x_pos, attrs;
    const char *name;
    const int len,
              current_id,
              up_id,
              down_id,
              left_id,
              right_id;
};

struct tui_textfield {
    int y_pos, x_pos, attrs;
    const char *name;
    const int len_name,
              len_buf,
              max_buf_len;
    char *buf;
    int buf_size;
    const int current_id,
              up_id,
              down_id,
              left_id,
              right_id;
};

void print_min_size (const int, const int, const int, const int);
void print_clear_win_at (const int, const int, const int, const int, const char *);
void print_tui_buttons (const int, const struct tui_button *, const int);
void print_tui_textfields (const int, const struct tui_textfield *, const int);
int find_next_item (const int, const MEVENT *, int *, const struct tui_button *, const int, const struct tui_textfield *, const int);
void enter_to_textfield (const int, const int, struct tui_textfield *, const int);
void cursor_to_textfield (const int, const struct tui_textfield *, const int);
int check_move_key (const int);
int check_enter_key (const int);

extern struct serv_item *serv_items_array;
extern unsigned int serv_items_array_len;

