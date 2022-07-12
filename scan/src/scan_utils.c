/*****************************************************************************
 *
 *    scan/src/scan_utils.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <json-c/json.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "scan/scan_utils.h"

int read_varint (const unsigned int);
int write_varint (int, char *);

const char request_packet [2] = { 0x01, 0x00 };
const char handshake_packet_id = 0x00;

int connection_init (const unsigned int sock, const char *ip, const unsigned short port, const int timeout) {
    struct timeval tv_timeout;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr (ip);
    addr.sin_port = htons (port);

    tv_timeout.tv_sec = timeout;
    tv_timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &tv_timeout, sizeof (struct timeval)) < 0) {
        close (sock);
        return 0;
    } if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, &tv_timeout, sizeof (struct timeval)) < 0) {
        close (sock);
        return 0;
    } if (connect (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        close (sock);
        return 0;
    }

    return 1;
}

/* I'm have VarInt*/
int send_request (const unsigned int sock, char **json_buf, const char *ip, const unsigned int ip_len, const unsigned short port, const int protocol) {
    char *handshake_buf,
         handshake_len_varint [5],
         protocol_varint [5],
         ip_len_varint [5],
         byte;

    int handshake_len,
        handshake_len_varint_len,
        protocol_varint_len,
        ip_len_varint_len,
        handshake_buf_pos = 0,
        json_len,
        json_tmp_len,
        json_bytes_read;

    /* Calculating Handshake Length */
    handshake_len = 4 + ip_len; /* 4 Bytes For Static Length Of Packet ID, Port And Next State Fields */
    handshake_len += protocol_varint_len = write_varint (protocol, protocol_varint);
    handshake_len += ip_len_varint_len = write_varint (ip_len, ip_len_varint);
    handshake_len_varint_len = write_varint (handshake_len, handshake_len_varint);
    handshake_buf = malloc (handshake_len + handshake_len_varint_len);

    /* Write VarInt Handshake Length */
    memcpy (handshake_buf + handshake_buf_pos, handshake_len_varint, handshake_len_varint_len);
    handshake_buf_pos += handshake_len_varint_len;

    /* Write Packet ID */
    handshake_buf [handshake_buf_pos++] = 0x00;

    /* Write VarInt Protocol */
    memcpy (handshake_buf + handshake_buf_pos, protocol_varint, protocol_varint_len);
    handshake_buf_pos += protocol_varint_len;

    /* Write VarInt Host Length */
    memcpy (handshake_buf + handshake_buf_pos, ip_len_varint, ip_len_varint_len);
    handshake_buf_pos += ip_len_varint_len;

    /* Write Host */
    memcpy (handshake_buf + handshake_buf_pos, ip, ip_len);
    handshake_buf_pos += ip_len;

    /* Write Port And Next State */
    handshake_buf [handshake_buf_pos++] = (port >> 8) & 0xFF;
    handshake_buf [handshake_buf_pos++] = port & 0xFF;
    handshake_buf [handshake_buf_pos++] = 0x01;

    if (send (sock, handshake_buf, handshake_len + handshake_len_varint_len, 0) < 0) {
        close (sock);
        free (handshake_buf);
        return 0;
    } free (handshake_buf);

    if (send (sock, request_packet, 2, 0) < 0) {
        close (sock);
        return 0;
    }

    read_varint (sock);
    if (read (sock, &byte, 1) < 0) {
        close (sock);
        return 0;
    } if (byte) {
        close (sock);
        return 0;
    }

    json_len = read_varint (sock);
    if (json_len < 1)
        return 0;
    *json_buf = malloc (json_len);

    json_tmp_len = 0;
    while (json_tmp_len < json_len) {
        if ((json_bytes_read = recv (sock, *json_buf + json_tmp_len, json_len - json_tmp_len, 0)) <= 0) {
            close (sock);
            free (*json_buf);
            return 0;
        } json_tmp_len += json_bytes_read;
    }

    close (sock);
    return json_len;
}

int read_varint (const unsigned int sock) {
    char byte;
    int value = 0,
        pos = 0;

    do {
        read (sock, &byte, 1);
        value |= (byte & 0x7F) << pos;
        pos += 7;
    } while (byte & 0x80 && pos < 32);
    return value;
}

int write_varint (int value, char *buf) {
    unsigned int pos;

    for (pos = 0; pos < 5; ++pos) {
        if (!(value & ~0x7F)) {
            buf [pos++] = value & 0x7F;
            break;
        } buf [pos] = (value & 0x7F) | 0x80;
        value = (unsigned int) value >> 7;
    } return pos;
}

