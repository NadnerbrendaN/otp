/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A non-standard implementation of a ChaCha-style cipher for stream encryption
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */
#include "chacha.hpp"

// non-standard constants because I wanted to do it this way
constexpr std::uint32_t CHA_CONST_0 = 0x6c756e61;
constexpr std::uint32_t CHA_CONST_1 = 0xC4AC4A20;
constexpr int ROUNDS = 20; 

std::uint32_t rotl(std::uint32_t num, std::uint32_t shift) {
    return ((num << shift) | (num >> (32 - shift)));
}

void qr(std::uint32_t &a, std::uint32_t &b, std::uint32_t &c, std::uint32_t &d) {
    a += b; d ^= a; d = rotl(d, 16);
    c += d; b ^= c; b = rotl(b, 12);
    a += b; d ^= a; d = rotl(d, 8);
    c += d; b ^= c; d = rotl(b, 7);
}

void chacha(std::uint32_t* out, std::uint32_t* in) {
    std::uint32_t x[16];

    x[0] = CHA_CONST_0;
    x[1] = CHA_CONST_1;

    for (int i = 0; i < 14; ++i) {
        x[i + 2] = in[i];
    }

    for (int i = 0; i < ROUNDS/2; ++i) {
        qr(x[0], x[4], x[8], x[12]);
        qr(x[1], x[5], x[9], x[13]);
        qr(x[2], x[6], x[10], x[14]);
        qr(x[3], x[7], x[11], x[15]);

        qr(x[0], x[5], x[10], x[15]);
        qr(x[1], x[6], x[11], x[12]);
        qr(x[2], x[7], x[8], x[13]);
        qr(x[3], x[4], x[9], x[14]);
    }

    for (int i = 0; i < 16; ++i) {
        out[i] = x[i] + in[i];
    }
}
