/*
 * Copyright (C) 2025 NadnerbrendaN <albertdock@duck.com>
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <cstdio>
#include <fstream>
#include <iostream>

enum Mode { // available options for the running mode
    UNSET,
    BYTE,
};

char enc_byte(char m, char k, bool e) { // encrypt one byte with another byte
    if (e) { // switch based on encrypt/decrypt
        return m+k;
    } else {
        return m-k;
    }
}

void complain(Mode mode) { // exit with errors and message based on mode
    switch (mode) {
        case UNSET: // no given mode --> explain that a mode is needed
            std::cout << "Usage: otp (mode) [FLAGS, FILES, and OPTIONS]\n\nMode possibilities:\n\
\tbyte (or anything starting with 'b')\n\t\tSets the system to BYTE mode.\n\
\n\tBYTE mode:\n\
\tEncrypts the message with the key byte by byte. The most basic mode. Required flags:\n\
\t\t-m (message file)\n\t\t-k (key file)\n\t\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n";
            break;
        case BYTE: // byte mode but one or more missing flags --> explain available flags
            std::cout << "BYTE mode requires these flags:\n\
\t-m (message file)\n\t-k (key file)\n\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n";
            break;
    }
}

int encrypt(Mode mode, char* message_name, char* key_name, char* out_name, bool enc, bool del, bool seed) {
    std::ifstream message_file; // a file input stream from which to read the message
    std::ifstream key_file; // a file input stream from which to read the key/seed
    std::ofstream out_file; // a file output stream to write the cyphertext to
    message_file.open(message_name);
    key_file.open(key_name);
    out_file.open(out_name);
    if (!message_file.is_open() || !key_file.is_open() || !out_file.is_open()) { // catch missing files
        int n = 0;
        if (!message_file.is_open()) {
            std::cout << "File not found: " << message_name << '\n';
            n++;
        }
        if (!key_file.is_open()) {
            std::cout << "File not found: " << key_name << '\n';
            n++;
        }
        if (!out_file.is_open()) n++; // (I think this technically shouldn't happen but whatever)
        message_file.close();
        key_file.close(); // close all files for propriety
        out_file.close();
        std::remove(out_name);
        return n;
    }

    char mch;
    char kch;
    bool key_left = true;
    while (message_file.get(mch) && key_left) { // while we have message and key left
        if (!key_file.get(kch)) { // if we can't read from the file
            std::cout << "Key too short:\nTry adding more data to the key or writing a shorter message.\n";
            key_left = false;
            std::remove(out_name);
            key_file.close();
            message_file.close();
            out_file.close();
            return 1;
        } else {
            if (enc) {
                out_file.put(enc_byte(mch, kch, false));
            } else {
                out_file.put(enc_byte(mch, kch, true));
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
    message_file.close(); // close our file streams for safety
    out_file.close();
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
           } else if (argv[i][1] == 'e') { // encrypt option
               enc = true;
           } else if (argv[i][1] == 'd') { // delete option
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
