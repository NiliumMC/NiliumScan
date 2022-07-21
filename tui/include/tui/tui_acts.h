/*****************************************************************************
 *
 *    tui/include/tui/tui_acts.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

struct action {
    char *name;
    int len;
    int (*func) (const MEVENT *, const int, const int, const int, const char *);
    char bind;
    int enabled;
};

void print_act (const int, const int, const struct action *); /* Draw Actions At Main Window */
int check_mouse_pos_acts (const int, const MEVENT *, const struct action *, const int); /* Check If Pressed Mouse Key At Actions Positions */

