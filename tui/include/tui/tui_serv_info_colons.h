/*
 *  tui/include/tui/tui_serv_info_colons.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

/* Main menu server column structure */
struct serv_info_colon {
    const char * const name;
    const int  name_str_len;
    const int  field_len;
    const char bind;
};

/* Draw all available server columns headers */
void print_serv_info_colons (const struct serv_info_colon colons_arr [], const int array_size);

