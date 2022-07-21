/*****************************************************************************
 *
 *    scan/include/scan/scan.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

/* One Scanned Server Element Structure */
struct serv_item {
    char *ip;
    unsigned short port;
    char *version,
         *motd;
    int online,
        slots;
    struct serv_item *next;
};

/* `start_scan ()` Arguments Structure */
struct scan_args {
    char *ip;
    unsigned int ip_len;
    int protocol, timeout;
    unsigned short port_fst;
    unsigned short port_lst;
};

void * start_scan (void *); /* Start Servers Scan */
void copy_serv_list_to_array (void); /* Move Scanned Servers From Linked List To An Array */
void free_serv_item (struct serv_item *); /* Free Memory Dedicated For Server Item Structure */

extern int is_scanning; /* Scan Toggle */
extern struct serv_item serv_items_list; /* Linked List Of Servers Elements */
extern struct serv_item *last_serv_list_item; /* Last Servers List Element */
extern int serv_items_list_len; /* Lenght Of Servers Linked List */

extern struct serv_item *serv_items_array; /* Dynamic Array Of Scanned Servers */
extern int serv_items_array_len; /* Lenght Of Previous Array */

