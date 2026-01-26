/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * The main source file for NadnerbrendaN's "otp" project, implementing one time pad encryption.
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
#include "chacha.hpp"

enum Mode { // available options for the running mode
    UNSET,
    BYTE,
    GEN,
};

void complain(Mode mode) { // send error message based on mode
    switch (mode) {
        case UNSET: // no given mode --> explain that a mode is needed
            std::cout << "Usage: otp (mode) [FLAGS, FILES, and OPTIONS]\n\nMode possibilities:\n\
\tbyte (or anything starting with 'b')\n\t\tSets the system to BYTE mode.\n\
\tgen (or anything starting with 'g')\n\t\tSets the system to GEN mode.\n\
\n\tBYTE mode:\n\
\tEncrypts the provided message with the provided key (or seed) byte by byte. Required flags:\n\
\t\t-m (message file)\n\t\t-k (key file)\n\t\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data. Incompatible with the seed option.\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n\
\t-s\n\t\tIndicate that the key is to be used as a seed for a ChaCha cipher.\n\
\t\tThis will disable the delete option.\n\
\t\tWARNING: This can significantly reduce the randomness of the key data, making the system less secure.\n\
\t\tThis should be used only if absolute security is unnecessary.\n\
\n\tGEN mode:\n\
\tGenerates keys or seeds based on the arguments specified. This is a work in progress. It doesn't function.\n\
";
            break;
        case BYTE: // byte mode but one or more missing flags --> explain available flags
            std::cout << "BYTE mode requires these flags:\n\
\t-m (message file)\n\t-k (key file)\n\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n";
            break;
        case GEN:
            std::cout << "This mode is as of yet undefined. It will randomly generate data or keys.\n";
            break;
    }
}

char enc_byte(char m, char k, bool e) { // encrypt one byte with another byte
    if (e) { // switch add/sub based on encrypt/decrypt
        return m+k;
    } else {
        return m-k;
    }
}

int encrypt(Mode mode, char* message_name, char* key_name, char* out_name, bool enc, bool del, bool seed) {
    std::ifstream message_file; // a file input stream from which to read the message
    std::ifstream key_file; // a file input stream from which to read the key/seed
    std::ofstream out_file; // a file output stream to write the cyphertext to
    message_file.open(message_name);
    key_file.open(key_name);
    out_file.open(out_name);
    int code = 0; // the error code to return
    /* 
     * I'm doing it this way so I can call functions that return error codes and then pass those
     * upstream without just immediately giving up and returning the code, so that I have time to
     * clean up or perform other tasks before erroring.
     */
    if (!message_file.is_open() || !key_file.is_open() || !out_file.is_open()) { // catch missing files
        if (!message_file.is_open()) {
            std::cout << "File not found: " << message_name << '\n';
            code++;
        }
        if (!key_file.is_open()) {
            std::cout << "File not found: " << key_name << '\n';
            code++;
        }
        if (!out_file.is_open()) code++; // (I think this technically shouldn't happen but whatever)
        message_file.close();
        key_file.close(); // close all files for propriety
        out_file.close();
        std::remove(out_name);
        return code;
    }

    if (mode == BYTE && !seed) {
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
    } else if (mode == BYTE && seed) {
        char sch;
        int count = 0;
        std::uint32_t seed_data[16] = {0};
        while (key_file.get(sch) && count < 48) {
            seed_data[2 + (count / 4)] += (((std::uint32_t) sch) << ((count % 4) * 8));
            ++count;
            if (key_file.peek() == -1) {
                key_file.seekg(0);
                break;
            }
        }
        if (count < 48) {
            std::ofstream temp_key(".keyfile.temp");
            char kch;
            while (key_file.peek() != -1 && key_file.get(kch)) {
                temp_key.put(kch);
            }
            for (int i = count; i < 48; ++i) {
                temp_key.put(0x00);
            }
            temp_key.close();
            std::rename(".keyfile.temp", key_name);
        } else {
            key_file.seekg(48);
            std::uint32_t nonce = 0;
            count = 0;
            char kch;
            while (key_file.peek() != -1 && key_file.get(kch) && count < 4) {
                nonce += (kch << (count * 8));
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
        key_file.seekg(0);
        std::ofstream temp_key(".keyfile.temp");
        char kch;
        count = 0;
        while (key_file.get(kch) && count < 48) {
            temp_key.put(kch);
            ++count;
        }
        temp_key.put(out[14] % 256);
        temp_key.put((out[14] >> 8) % 256);
        temp_key.put((out[14] >> 16) % 256);
        temp_key.put((out[14] >> 24) % 256);
        temp_key.close();
        std::rename(".keyfile.temp", key_name);
    }

    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        complain(UNSET);
        return 1;
    }
    int i = 2; // current position from 0 to argc in argv
    char* message_name = {0}; // path to the message file
    char* key_name = {0}; // path to the key/seed file
    char* out_name = {0}; // path to the desired output file
    Mode mode;
    bool enc = false; // decrypt by default
    bool del = false; // save key data by default
    bool seed = false; // read key as a seed instead of raw key data
    switch (argv[1][0]) { // read the first argument as a mode
        case 'b':
            mode = BYTE;
            break;
        case 'g':
            mode = GEN;
            break;
        default:
            complain(UNSET);
            return 1;
    }
    while (i < argc) { // loop until out of inputs
        if (argv[i][0] == '-') { // check for flags/options
           if (argv[i][1] == 'm' && argc > i+1) { // message file
               ++i;
               message_name = argv[i];
           } else if (argv[i][1] == 'k' && argc > i+1) { // key file
               ++i;
               key_name = argv[i];
           } else if (argv[i][1] == 'o' && argc > i+1) { // cyphertext file
               ++i;
               out_name = argv[i];
           } else if (argv[i][1] == 's') { // seed option
               seed = true;
               del = false;
           } else if (argv[i][1] == 'e') { // encrypt option
               enc = true;
           } else if (argv[i][1] == 'd' && !seed) { // delete option
               del = true;
           }
        }
        ++i;
    }

    if (mode == BYTE) {
        if (message_name == 0 || key_name == 0 || out_name == 0) {
            int n = 0;
            if (message_name == 0) n++;
            if (key_name == 0) n++;
            if (out_name == 0) n++;
            complain(mode);
            return n;
        }
        return encrypt(mode, message_name, key_name, out_name, enc, del, seed);
    }
}
