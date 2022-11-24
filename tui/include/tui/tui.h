/*
 *  tui/include/tui/tui.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <ncurses.h>

#include <stdbool.h>

/* Starts the TUI Mode.
 * Initializing Ncurses.
 *
 * Returns true if successful, otherwise false.
 */
bool start_tui (void);

/* Deinitialize the Ncurses. */
void end_tui (void);

/* Starts the TMCScan TUI session.
 * Draw main menu.
 */
void show_menu (void);

