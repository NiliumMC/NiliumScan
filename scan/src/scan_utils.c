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
#include "scan/scan_varint.h"

const char request_packet [2] = { 0x01, 0x00 }; /* Second C2S Packet (Request) */
const char handshake_packet_id = 0x00; /* Packet ID Of First C2S Packet (Handshake) */

int connection_init (const int sock, const char *ip, const unsigned short port, const int timeout) {
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

/* I'm hate VarInt*/
int send_request (const int sock, char **json_buf, const char *ip, const int ip_len, const unsigned short port, const int protocol) {
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
    handshake_buf = malloc ((unsigned int) (handshake_len + handshake_len_varint_len));

    /* Write VarInt Handshake Length */
    memcpy (handshake_buf + handshake_buf_pos, handshake_len_varint, (unsigned int) handshake_len_varint_len);
    handshake_buf_pos += handshake_len_varint_len;

    /* Write Packet ID */
    handshake_buf [handshake_buf_pos++] = 0x00;

    /* Write VarInt Protocol */
    memcpy (handshake_buf + handshake_buf_pos, protocol_varint, (unsigned int) protocol_varint_len);
    handshake_buf_pos += protocol_varint_len;

    /* Write VarInt Host Length */
    memcpy (handshake_buf + handshake_buf_pos, ip_len_varint, (unsigned int) ip_len_varint_len);
    handshake_buf_pos += ip_len_varint_len;

    /* Write Host */
    memcpy (handshake_buf + handshake_buf_pos, ip, (unsigned int) ip_len);
    handshake_buf_pos += ip_len;

    /* Write Port And Next State */
    handshake_buf [handshake_buf_pos++] = (((char) port) >> 8) & 0xFF;
    handshake_buf [handshake_buf_pos++] = (char) (port & 0xFF);
    handshake_buf [handshake_buf_pos++] = 0x01;

    if (send (sock, handshake_buf, (unsigned int) (handshake_len + handshake_len_varint_len), 0) < 0) {
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
    if (json_len < 1 || json_len > MAX_SERV_RESPONSE_LEN)
        return 0;
    *json_buf = malloc ((unsigned int) json_len);

    json_tmp_len = 0;
    while (json_tmp_len < json_len) {
        if ((json_bytes_read = (int) recv (sock, *json_buf + json_tmp_len, (unsigned int) (json_len - json_tmp_len), 0)) <= 0) {
            close (sock);
            free (*json_buf);
            return 0;
        } json_tmp_len += json_bytes_read;
    }

    close (sock);
    return json_len;
}

