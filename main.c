/*****************************************************************************
 *
 *    main.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <stdio.h>

#include "tui/tui.h"

int main (void) {
    if (ini_curses ()) {
        show_menu ();
        fin_curses ();
    } else {
        fin_curses ();
        fputs ("Curses not initialized normally!\n", stderr);
        return 1;
    } return 0;
}

