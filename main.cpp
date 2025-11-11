/*
 * Copyright (C) 2025 NadnerbrendaN <albertdock@duck.com>
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

char encrypt(char m, char k, bool e) {
    if (e) {
        return m+k;
    } else {
        return m-k;
    }
}

int main(int argc, char** argv) {
    int i = 1;
    std::ifstream message_file; // a file input stream to hold the message
    std::ifstream key_file; // a file input/output stream to use the key
    char* key_name;
    std::ofstream out_file; // a file output stream to write the cyphertext
    char* out_name;
    bool enc = false;
    bool del = false;
    while (i < argc) { // loop until out of inputs
        if (argv[i][0] == '-' && argv[i][1] == 'm' && argc > i+1) { // message file
            ++i;
            message_file.open(argv[i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'k' && argc > i+1) { // key file
            ++i;
            key_name = argv[i];
            key_file.open(key_name);
        } else if (argv[i][0] == '-' && argv[i][1] == 'o' && argc > i+1) { // cyphertext file
            ++i;
            out_file.open(argv[i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'e') {
            enc = true;
        } else if (argv[i][0] == '-' && argv[i][1] == 'd') {
            del = true;
        }
        ++i;
    }
    if (!message_file.is_open() || !key_file.is_open() || !out_file.is_open()) {
        // complain if any files aren't open
        std::cout << "Usage: otp [FLAGS, FILES, and OPTIONS]\n\nRequired flags:\n\
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
        } else {
            out_file.put(encrypt(mch, kch, enc));
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
