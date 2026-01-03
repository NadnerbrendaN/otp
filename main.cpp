/*
 * Copyright (C) 2025 NadnerbrendaN <albertdock@duck.com>
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>

enum Mode {
    UNSET,
    SET_SEED,
    ASCII,
};

char enc_ascii(char m, char k, bool e) {
    if (e) {
        return m+k;
    } else {
        return m-k;
    }
}

int encrypt(Mode mode, char* message_name, char* key_name, char* out_name, bool enc, bool del) {
    std::ifstream message_file; // a file input stream from which to read the message
    std::ifstream key_file; // a file input stream from which to read the key
    std::ofstream out_file; // a file output stream to write the cyphertext to
    message_file.open(message_name);
    key_file.open(key_name);
    out_file.open(out_name);
    if (!message_file.is_open() || !key_file.is_open() || !out_file.is_open()) {
        // complain if any files aren't open
        std::cout << "ASCII mode requires these flags:\n\
-m (message file)\n-k (key file)\n-o (output file)\n\nOptions:\n\
-d\n    Delete used key data\n\
-e\n    Encrypt the message, instead of the default decryption setting.\n";
        return 1;
    }

    char mch;
    char kch;
    bool key_left = true;
    while (message_file.get(mch) && key_left) {
        if (!key_file.get(kch)) {
            std::cout << "Key too short:\nTry adding more data to the key or writing a shorter message.\n";
            key_left = false;
            std::remove(out_name);
            key_file.close();
            message_file.close();
            out_file.close();
            return 1;
        } else {
            if (enc) {
                out_file.put(enc_ascii(mch, kch, false));
            } else {
                out_file.put(enc_ascii(mch, kch, true));
            }
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
    key_file.close();
    message_file.close();
    out_file.close();

    return 0;
}

int main(int argc, char** argv) {
    int i = 1;
    char* message_name;
    char* key_name;
    char* out_name;
    char* seed_name;
    Mode mode = UNSET;
    bool enc = false;
    bool del = false;
    bool seed = false;
    while (i < argc) { // loop until out of inputs
        if (argv[i][0] == '-' && argv[i][1] == 'm' && argc > i+1) { // message file
            ++i;
            message_name = argv[i];
        } else if (argv[i][0] == '-' && argv[i][1] == 'k' && argc > i+1) { // key file
            ++i;
            key_name = argv[i];
        } else if (argv[i][0] == '-' && argv[i][1] == 'o' && argc > i+1) { // cyphertext file
            ++i;
            out_name = argv[i];
        } else if (argv[i][0] == '-' && argv[i][1] == 'S' && argc > i+1) { // use seed in given file
            ++i;
            seed_name = argv[i];
        } else if (argv[i][0] == '-' && argv[i][1] == 'e') { // encrypt option
            enc = true;
        } else if (argv[i][0] == '-' && argv[i][1] == 'd') { // delete option
            del = true;
        } else if (argv[i][0] == '-' && argv[i][1] == 's' && argc > i+1) { // set mode flag
            ++i;
            if (argv[i][0] == 's') { // set seed for future random keys
                mode = SET_SEED;
            } else if (argv[i][0] == 'a') {// normal set-key ascii mode
                mode = ASCII;
            }
        }
        ++i;
    }

    if (mode == UNSET) { // if they didn't set a mode
        std::cout << "Usage: otp [FLAGS, FILES, and OPTIONS]\n\nRequired flags:\n\
\t-s (set mode), which takes the following options:\n\
\t\ta\n\t\t\tSets the system to ASCII mode.\n\
\t\ts\n\t\t\tSets the system to SET_SEED mode\n\
\t\tu\n\t\t\tSets the system to USE_SEED mode\n\n\tASCII mode:\n\
\t\t-m (message file)\n\t\t-k (key file)\n\t\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n";
        return 1;
    }
    
    if (mode == ASCII) {
        return encrypt(mode, message_name, key_name, out_name, enc, del);
    }
}
