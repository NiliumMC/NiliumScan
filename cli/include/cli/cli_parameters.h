/*
 *  cli/include/cli/cli_parameters.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/0Magenta0/TMCScan
 */

enum term_flags_bitset {
    TFB_USE_CLI = 0x01
};

extern enum term_flags_bitset term_flags;

void handle_parameters (const int, char ** const);

