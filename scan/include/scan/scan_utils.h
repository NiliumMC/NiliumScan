/*****************************************************************************
 *
 *    scan/include/scan/scan_utils.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#define MAX_SERV_RESPONSE_LEN 16384 /* Maximum Lenght Of Server's JSON Response */

int connection_init (const unsigned int, const char *, const unsigned short, const int); /* Init Connection To The Server */
int send_request (const unsigned int, char **, const char *, const unsigned int, const unsigned short, const int); /* Send Handshake/Request To The Server */

