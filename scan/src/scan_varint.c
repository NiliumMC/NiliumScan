/*****************************************************************************
 *
 *    scan/src/scan_varint.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

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

