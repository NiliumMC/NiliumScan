/*****************************************************************************
 *
 *    tui/include/tui/tui_filter.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#define MAX_FILTER_FIELD_LEN 20
#define MAX_FILTER_STR_LEN 35

extern int is_filtering;
extern int is_entering_filter;

extern int *filtered_indexes_array,
           filtered_indexes_array_len,
           current_filtered_serv_item,
           filtered_items_shift;

void print_filter (const int, const int);
int filter_key_handler (const int);
void cursor_to_filter (const int, const int);
void disable_entering_filter (void);
void drop_filter (void);
int check_mouse_pos_filter (const MEVENT *);
int check_mouse_pos_filter_ok_button (const int, const int, const MEVENT *);
int check_mouse_pos_filter_dl_button (const int, const int, const MEVENT *);

