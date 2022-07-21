/*****************************************************************************
 *
 *    scan/src/scan.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <json-c/json.h>
#include <pthread.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>

#include "scan/scan_utils.h"
#include "scan/scan.h"

#define STR_NULL "[NULL]" /* Placeholder For Empty Server Element's Fields */
#define STR_NULL_LEN 7 /* Lenght Of Previous String */

int get_serv (struct scan_args *, const unsigned short); /* Ping Certain Server */
char *copy_json_string (const char *, const unsigned int); /* Copy String From JSON Response */

int is_scanning;

pthread_mutex_t scan_mutex; /* Mutex Lock For Scan Function */

struct serv_item serv_items_list;
struct serv_item *last_serv_list_item;
int serv_items_list_len;

struct serv_item *serv_items_array;
int serv_items_array_len;

void * start_scan (void *sargs) {
    static unsigned short port_now;

    if (is_scanning && !port_now) {
        port_now = ((struct scan_args *) sargs)->port_fst;
    } while (is_scanning && !pthread_mutex_lock (&scan_mutex) && port_now <= ((struct scan_args *) sargs)->port_lst) {
        get_serv ((struct scan_args *) sargs, port_now++);
        if (port_now == 0) {
            pthread_mutex_unlock (&scan_mutex);
            break;
        }
    }

    pthread_mutex_unlock (&scan_mutex);
    is_scanning = 0;
    port_now = 0;
    return sargs;
}

/* TODO: Code refactoring */
int get_serv (struct scan_args *sargs, const unsigned short port) {
    pthread_mutex_unlock (&scan_mutex);
    struct serv_item *this_serv_item;
    int sock;

    char *json_buf = 0;
    int json_buf_len;
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

    if (serv_items_list_len) {
        this_serv_item = malloc (sizeof (struct serv_item));
        bzero (this_serv_item, sizeof (struct serv_item));
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

        pthread_mutex_lock (&scan_mutex);
        last_serv_list_item->next = this_serv_item;
        last_serv_list_item = this_serv_item;
        ++serv_items_list_len;
        pthread_mutex_unlock (&scan_mutex);
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

        pthread_mutex_lock (&scan_mutex);
        last_serv_list_item = &serv_items_list;
        ++serv_items_list_len;
        pthread_mutex_unlock (&scan_mutex);
    }

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
    char *tmp_char_ptr;

    if (src_serv->ip) {
        dest_serv->ip = malloc (strlen (src_serv->ip) + 1);
        strcpy (dest_serv->ip, src_serv->ip);
    }

    dest_serv->port = src_serv->port;
    if (src_serv->version) {
        dest_serv->version = malloc (strlen (src_serv->version) + 1);
        strcpy (dest_serv->version, src_serv->version);
        while ((tmp_char_ptr = strchr (dest_serv->version, 0x0A))) {
            *tmp_char_ptr = ' ';
        }
    } if (src_serv->motd) {
        dest_serv->motd = malloc (strlen (src_serv->motd) + 1);
        strcpy (dest_serv->motd, src_serv->motd);
        while ((tmp_char_ptr = strchr (dest_serv->motd, 0x0A))) {
            *tmp_char_ptr = ' ';
        }
    }

    dest_serv->online = src_serv->online;
    dest_serv->slots = src_serv->slots;
    dest_serv->next = 0;
}

void copy_serv_list_to_array (void) {
    struct serv_item *tmp_item_ptr;
    int i = 0;

    last_serv_list_item = 0;
    serv_items_array_len = serv_items_list_len;
    serv_items_array = malloc (sizeof (struct serv_item) * serv_items_array_len);
    last_serv_list_item = &serv_items_list;
    for (i = 0; i < serv_items_list_len; ++i) {
        copy_serv_to_another (&serv_items_array [i], last_serv_list_item);
        last_serv_list_item = last_serv_list_item->next;
    }

    if (serv_items_list.next) {
        last_serv_list_item = serv_items_list.next;
        for (i = 0; i < serv_items_list_len - 1; ++i) {
            /* if (!last_serv_list_item) break; */
            tmp_item_ptr = last_serv_list_item->next;
            free_serv_item (last_serv_list_item);
            free (last_serv_list_item);
            last_serv_list_item = tmp_item_ptr;
        }
    }

    free_serv_item (&serv_items_list);
    serv_items_list_len = 0;
}

void free_serv_item (struct serv_item *serv_item_ptr) {
    if (serv_item_ptr->ip) {
        if (serv_item_ptr->ip != serv_items_list.ip)
            free (serv_item_ptr->ip);
        serv_item_ptr->ip = 0;
    }

    serv_item_ptr->port = 0;
    if (serv_item_ptr->version) {
        free (serv_item_ptr->version);
        serv_item_ptr->version = 0;
    } if (serv_item_ptr->motd) {
        free (serv_item_ptr->motd);
        serv_item_ptr->motd = 0;
    }

    serv_item_ptr->online = 0;
    serv_item_ptr->slots = 0;
    serv_item_ptr->next = 0;
}

