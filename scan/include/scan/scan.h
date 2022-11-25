/*
 *  scan/include/scan/scan.h
 *  This file is a part of the TMCScan distribution released under GNU GPLv3
 *  Copyright (C) 2022 _Magenta_
 *  https://github.com/TuiMC/TMCScan
 */

#include <stdbool.h>

#include <netinet/in.h>

enum start_status {
    SS_SUCCESS,
};

struct serv_online {
    int32_t now_players;
    int32_t max_players;
};

struct serv_node {
    struct serv_node *next;
    struct sockaddr_in addr;
    struct serv_online online;
    char *str_version;
    size_t str_version_len;
    char *str_motd;
    size_t str_motd_len;
};

extern bool is_scanning;

enum start_status start_scan (void);

