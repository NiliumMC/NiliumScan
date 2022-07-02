/*****************************************************************************
 *
 *    scan/src/scan.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <json-c/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "scan/scan.h"
#include "scan/scan_utils.h"

#define STR_NULL "[NULL]"
#define STR_NULL_LEN 7

int is_scanning;

struct serv_item serv_items_list;
struct serv_item *last_serv_list_item;
unsigned int serv_items_list_len;

struct serv_item *serv_items_array;
unsigned int serv_items_array_len;

int get_serv (struct scan_args *, const unsigned short);
char *copy_json_string (const char *, const unsigned int);
void copy_serv_to_another (struct serv_item *, const struct serv_item *);

/* TODO: Check if mutex is needed */
void * start_scan (void * sargs) {
    static unsigned short port_now;

    if (is_scanning && !port_now) {
        port_now = ((struct scan_args *) sargs)->port_fst;
    } while (is_scanning && port_now <= ((struct scan_args *) sargs)->port_lst) {
        get_serv ((struct scan_args *) sargs, port_now++);
        if (port_now == 0)
            break;
    }

    is_scanning = 0;
    port_now = 0;
    return sargs;
}

/* TODO: Code refactoring */
int get_serv (struct scan_args *sargs, const unsigned short port) {
    struct serv_item *this_serv_item;
    int sock;

    char *json_buf = 0;
    unsigned int json_buf_len;
    struct json_tokener *tokener_ex;
    struct json_object *parsed_json,
                       *certain_json_obj;

    if ((sock = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
        return 0;
    } if (!connection_init (sock, sargs->ip, port, sargs->timeout)) {
        return 0;
    } if (!(json_buf_len = send_request (sock, &json_buf, sargs->ip, sargs->ip_len, port, sargs->protocol))) {
        return 0;
    }

    tokener_ex = json_tokener_new ();
    if (!(parsed_json = json_tokener_parse_ex (tokener_ex, json_buf, json_buf_len))) {
        free (json_buf);
        return 0;
    } json_tokener_free (tokener_ex);

    this_serv_item = malloc (sizeof (struct serv_item));
    bzero (this_serv_item, sizeof (struct serv_item));
    if (serv_items_list_len) {
        this_serv_item->ip = sargs->ip;
        this_serv_item->port = port;

        certain_json_obj = json_object_object_get (parsed_json, "version");
        this_serv_item->version = copy_json_string (json_object_get_string (json_object_object_get (certain_json_obj, "name")),
                                               json_object_get_string_len (json_object_object_get (certain_json_obj, "name")));

        certain_json_obj = json_object_object_get (parsed_json, "description");
        this_serv_item->motd = copy_json_string (json_object_get_string (json_object_object_get (certain_json_obj, "text")),
                                            json_object_get_string_len (json_object_object_get (certain_json_obj, "text")));

        certain_json_obj = json_object_object_get (parsed_json, "players");
        this_serv_item->online = json_object_get_int (json_object_object_get (certain_json_obj, "online"));
        this_serv_item->slots = json_object_get_int (json_object_object_get (certain_json_obj, "max"));

        last_serv_list_item->next = this_serv_item;
        last_serv_list_item = this_serv_item;
        ++serv_items_list_len;
    } else {
        serv_items_list.ip = sargs->ip;
        serv_items_list.port = port;

        certain_json_obj = json_object_object_get (parsed_json, "version");
        serv_items_list.version = copy_json_string (json_object_get_string (json_object_object_get (certain_json_obj, "name")),
                                               json_object_get_string_len (json_object_object_get (certain_json_obj, "name")));

        certain_json_obj = json_object_object_get (parsed_json, "description");
        serv_items_list.motd = copy_json_string (json_object_get_string (json_object_object_get (certain_json_obj, "text")),
                                            json_object_get_string_len (json_object_object_get (certain_json_obj, "text")));

        certain_json_obj = json_object_object_get (parsed_json, "players");
        serv_items_list.online = json_object_get_int (json_object_object_get (certain_json_obj, "online"));
        serv_items_list.slots = json_object_get_int (json_object_object_get (certain_json_obj, "max"));
        last_serv_list_item = &serv_items_list;
        ++serv_items_list_len;
    }

    /* free (this_serv_item); */
    json_object_put (parsed_json);
    free (json_buf);
    return 1;
}

char *copy_json_string (const char *json_str, const unsigned int len) {
    char *str;

    if (json_str && len) {
        str = malloc (len + 1);
        bzero (str, len + 1);
        strncpy (str, json_str, len);
    } else {
        str = malloc (STR_NULL_LEN);
        bzero (str, STR_NULL_LEN);
        strncpy (str, STR_NULL, STR_NULL_LEN);
    } return str;
}

void copy_serv_to_another (struct serv_item *dest_serv, const struct serv_item *src_serv) {
    if (src_serv->ip) {
        dest_serv->ip = malloc (strlen (src_serv->ip) + 1);
        strcpy (dest_serv->ip, src_serv->ip);
    }

    dest_serv->port = src_serv->port;
    if (src_serv->version) {
        dest_serv->version = malloc (strlen (src_serv->version) + 1);
        strcpy (dest_serv->version, src_serv->version);
    } if (src_serv->motd) {
        dest_serv->motd = malloc (strlen (src_serv->motd) + 1);
        strcpy (dest_serv->motd, src_serv->motd);
    }

    dest_serv->online = src_serv->online;
    dest_serv->slots = src_serv->slots;
    dest_serv->next = 0;
}

void copy_serv_list_to_array (void) {
    struct serv_item *tmp_item_ptr;
    unsigned int i = 0;

    last_serv_list_item = 0;
    serv_items_array_len = serv_items_list_len;
    serv_items_array = malloc (sizeof (struct serv_item) * serv_items_array_len);
    for (i = 0; i < serv_items_list_len; ++i) {
        if (!last_serv_list_item) {
            last_serv_list_item = &serv_items_list;
        } copy_serv_to_another (&serv_items_array [i], last_serv_list_item);
        last_serv_list_item = last_serv_list_item->next;
    }

    if (serv_items_list.next) {
        last_serv_list_item = serv_items_list.next;
        for (i = 0; i < serv_items_list_len - 1; ++i) {
            /* if (!last_serv_list_item) break; */
            tmp_item_ptr = last_serv_list_item->next;
            free (last_serv_list_item);
            last_serv_list_item = tmp_item_ptr;
        }
    }

    serv_items_list.ip = 0;
    serv_items_list.port = 0;
    if (serv_items_list.version) {
        free (serv_items_list.version);
    } if (serv_items_list.motd) {
        free (serv_items_list.motd);
    }

    serv_items_list.online = 0;
    serv_items_list.slots = 0;
    serv_items_list.next = 0;

    serv_items_list_len = 0;
}

