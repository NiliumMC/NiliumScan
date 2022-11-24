/*
 *  cli/include/cli/cli_parameters.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

/* List of switchable functions. */
enum term_flags_bitset {
    TFB_USE_CLI = 0x01
};

/* Bitset of the toggeled functions. */
extern enum term_flags_bitset term_flags;

/* Handle the terminal parameters.
 * Prints Help message if error.
 */
void handle_parameters (const int argc, char ** const argv);

