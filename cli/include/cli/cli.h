/*
 *  cli/include/cli/cli.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 0Magenta0
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

/* Pass term arguments to the handler.
 * Start the CLI Mode if needed.
 *
 * Returns true if CLI Mode is used, otherwise false.
 */
bool start_cli (const int argc, char ** const argv);

