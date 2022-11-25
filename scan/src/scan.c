/*
 *  scan/src/scan.c
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <pthread.h>

#include "scan/scan.h"

bool is_scanning;

enum start_status start_scan (void) {
    return SS_SUCCESS;
}

