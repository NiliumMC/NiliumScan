/*****************************************************************************
 *
 *    tui/include/tui/tui_scan.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

enum scan_status_enum {
    scan_status_idle,
    scan_status_error,
    scan_status_scanning,
    scan_status_end
};

int act_scan (const MEVENT *, const int, const int, const int, const char *); /* Bind Function For Scan Action */

extern pthread_t *scan_threads;
extern int scan_status;

