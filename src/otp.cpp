/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * The primary file containing encryption and decryption logic for one time pads and a stream
 * cipher
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not receive a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <cstdio>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <iostream>
#include "otp.hpp"
#include "chacha.hpp"

int unseeded_byte(const char* message_name, const char* key_name, const char* out_name, bool del) {
    std::ifstream message_file(message_name);
    std::ifstream key_file(key_name);
    std::ofstream out_file(out_name);
    if (!message_file.is_open()) {
        std::cout << "File not found: " << message_name << '\n';
        return 1;
    }
    if (!key_file.is_open()) {
        std::cout << "File not found: " << key_name << '\n';
        return 1;
    }
    if (!out_file.is_open()) return -1; // (I think this technically shouldn't happen but whatever)

    char mch;
    char kch;
    bool key_left = true;
    while (message_file.get(mch) && key_left) { // while we have message and key left
        if (!key_file.get(kch)) { // if we can't read from the file
            std::cout << "Key too short:\n\
                Try adding more data to the key or writing a shorter message.\n";
            key_left = false;
            std::remove(out_name);
            key_file.close();
            message_file.close();
            out_file.close();
            return 1;
        } else {
            out_file.put(mch ^ kch);
        }
    }
    if (key_left && del) {
        std::ofstream temp_key(".keyfile.temp");
        while (key_file.get(kch)) {
            temp_key.put(kch);
        }
        temp_key.close();
        std::rename(".keyfile.temp", key_name);
    }
    return 0;
}

int seeded_byte(const char* message_name, const char* seed_name, const char* out_name) {
    std::ifstream message_file(message_name);
    std::ifstream seed_file(seed_name);
    std::ofstream out_file(out_name);
    if (!message_file.is_open()) {
        std::cout << "File not found: " << message_name << '\n';
        return 1;
    }
    if (!seed_file.is_open()) {
        std::cout << "File not found: " << seed_name << '\n';
        return 1;
    }
    if (!out_file.is_open()) return -1; // (I think this technically shouldn't happen but whatever)

    int count = 0;
    char seed_data[48] = {0};
    while (seed_file.get(seed_data[count]) && count < 48) {
        ++count;
    }
    seed_file.close();
    char mch;
    std::uint32_t input[14] = {0};
    std::memcpy(input, seed_data, 48);
    std::uint32_t out[16] = {0};
    while (true) {
        chacha(out, input);
        for (int i = 0; i < 16; ++i) {
            for (int k = 0; k < 4; ++k) {
                if (message_file.get(mch)) {
                    out_file.put(mch ^ ((out[i] >> k*8) % 256));
                } else {
                    goto end_loops;
                }
            }
        }
        ++input[13];
    }
end_loops:
    std::ofstream seed_out(seed_name); // don't overwrite the file
    for (int i = 0; i < 40; ++i) {
        seed_out.put(seed_data[i]);
    }
    seed_out.put(out[14] % 256);
    seed_out.put((out[14] >> 8) % 256);
    seed_out.put((out[14] >> 16) % 256);
    seed_out.put((out[14] >> 24) % 256);
    seed_out.close();
    return 0;
}
