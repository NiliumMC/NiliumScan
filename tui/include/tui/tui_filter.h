/*****************************************************************************
 *
 *    tui/include/tui/tui_filter.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#define MAX_FILTER_FIELD_LEN 20 /* Maximum Servers List Filter's TUI Lenght */
#define MAX_FILTER_STR_LEN 35 /* Maximum Servers List Filter's String Lenght */

extern int is_filtering; /* Servers List Filtering Toggle */
extern int is_entering_filter; /* Entering Servers List Filter Toggle */

extern int *filtered_indexes_array, /* Dynamic Array Of Indexes Servers List Items */
           filtered_indexes_array_len, /* Lenght Of Previous Array */
           current_filtered_serv_item, /* Selected Now Filtered Servers List Item*/
           filtered_items_shift; /* Filtered Servers List Offset */

void print_filter (const int, const int); /* Draw Servers Filter At Main Window */
int filter_key_handler (const int); /* Handler Keys Pressed When Servers Filter Is Entering */
void cursor_to_filter (const int, const int); /* Move Terminal Cursor To Entering Servers Filter */
void disable_entering_filter (void); /* Stop Entering Servers Filter */
void drop_filter (void); /* Reset Servers Filter Value */
int check_mouse_pos_filter (const MEVENT *); /* Check If Pressed Mouse Key At Servers Filter */
int check_mouse_pos_filter_ok_button (const int, const int, const MEVENT *); /* Check If Pressed Mouse Key At Servers Filter OK Button */
int check_mouse_pos_filter_dl_button (const int, const int, const MEVENT *); /* Check If Pressed Mouse Key At Servers Filter DL Button */

