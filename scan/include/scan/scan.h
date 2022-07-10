/*****************************************************************************
 *
 *    scan/include/scan/scan.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

struct serv_item {
    char *ip;
    unsigned short port;
    char *version,
         *motd;
    int online,
        slots;
    struct serv_item *next;
};

struct scan_args {
    char *ip;
    unsigned int ip_len;
    int protocol, timeout;
    unsigned short port_fst;
    unsigned short port_lst;
};

void * start_scan (void *);
void copy_serv_list_to_array (void);
void free_serv_item (struct serv_item *);

extern int is_scanning;
extern struct serv_item serv_items_list;
extern struct serv_item *last_serv_list_item;
extern int serv_items_list_len;

extern struct serv_item *serv_items_array;
extern int serv_items_array_len;

