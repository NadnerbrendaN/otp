/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * The primary file containing encryption and decryption logic for one time pads and a stream
 * cipher
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <cstdio>
#include <fstream>
#include <cstdint>
#include <iostream>
#include "chacha.cpp"

char enc_byte(char m, char k, bool e) { // encrypt one byte with another byte
    if (e) { // switch add/sub based on encrypt/decrypt
        return m+k;
    } else {
        return m-k;
    }
}

int unseeded_byte(char* message_name, char* key_name, char* out_name, bool enc, bool del) {
    std::ifstream message_file; // a file input stream from which to read the message
    std::ifstream key_file; // a file input stream from which to read the key
    std::ofstream out_file; // a file output stream to write the cyphertext to
    message_file.open(message_name);
    key_file.open(key_name);
    out_file.open(out_name);
    if (!message_file.is_open() || !key_file.is_open() || !out_file.is_open()) { // catch missing files
        if (!message_file.is_open()) {
            std::cout << "File not found: " << message_name << '\n';
            return 4;
        }
        if (!key_file.is_open()) {
            std::cout << "File not found: " << key_name << '\n';
            return 5;
        }
        if (!out_file.is_open()) return 5; // (I think this technically shouldn't happen but whatever)
    }

    int code = 0;
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
            out_file.put(enc_byte(mch, kch, enc));
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

int seeded_byte(char* message_name, char* seed_name, char* out_name, bool enc) {
    std::ifstream message_file; // a file input stream from which to read the message
    std::ifstream seed_file; // a file input stream from which to read the seed
    std::ofstream out_file; // a file output stream to write the cyphertext to
    message_file.open(message_name);
    seed_file.open(seed_name);
    out_file.open(out_name);
    if (!message_file.is_open() || !seed_file.is_open() || !out_file.is_open()) { // catch missing files
        if (!message_file.is_open()) {
            std::cout << "File not found: " << message_name << '\n';
            return 4;
        }
        if (!seed_file.is_open()) {
            std::cout << "File not found: " << seed_name << '\n';
            return 5;
        }
        if (!out_file.is_open()) return 5; // (I think this technically shouldn't happen but whatever)
    }

    char sch;
    int count = 0;
    std::uint32_t seed_data[16] = {0};
    while (seed_file.get(sch) && count < 48) {
        seed_data[2 + (count / 4)] += (((std::uint32_t) sch) << ((count % 4) * 8));
        ++count;
        if (seed_file.peek() == -1) {
            seed_file.seekg(0);
            break;
        }
    }
    if (count < 48) {
        std::ofstream temp_key(".seed.temp");
        while (seed_file.peek() != -1 && seed_file.get(sch)) {
            temp_key.put(sch);
        }
        for (int i = count; i < 48; ++i) {
            temp_key.put(0x00);
        }
        temp_key.close();
        std::rename(".seed.temp", seed_name);
    } else {
        seed_file.seekg(48);
        std::uint32_t nonce = 0;
        count = 0;
        sch = 0;
        while (seed_file.peek() != -1 && seed_file.get(sch) && count < 4) {
            nonce += (sch << (count * 8));
            ++count;
        }
        seed_data[14] = nonce;
    }
    std::uint32_t out[16];
    char mch;
    bool run = true;
    while (run) {
        chacha(out, seed_data);
        for (int i = 0; i < 16; ++i) {
            for (int k = 0; k < 4; ++k) {
                if (run && message_file.get(mch)) {
                    out_file.put(enc_byte(mch, (out[i] >> k*8) % 256, enc));
                } else {
                    run = false;
                    break;
                }
            }
        }
        ++seed_data[15];
    }
    seed_file.seekg(0);
    std::ofstream temp_key(".seed.temp");
    sch = 0;
    count = 0;
    while (seed_file.get(sch) && count < 48) {
        temp_key.put(sch);
        ++count;
    }
    temp_key.put(out[14] % 256);
    temp_key.put((out[14] >> 8) % 256);
    temp_key.put((out[14] >> 16) % 256);
    temp_key.put((out[14] >> 24) % 256);
    temp_key.close();
    std::rename(".seed.temp", seed_name);
    return 0;
}
