/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A simple CLI to that allows users to encrypt and decrypt files using one time pads or a
 * stream cipher.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <iostream>
#include "otp.hpp"

enum Mode { // available options for the running mode
    UNSET,
    BYTE,
};

void complain(Mode mode) { // send error message based on mode
    switch (mode) {
        case UNSET:
            std::cout << "Usage: otp (mode) [FLAGS, FILES, and OPTIONS]\n\nMode possibilities:\n\
\tbyte (or anything starting with 'b')\n\t\tSets the system to BYTE mode.\n\
\n\tBYTE mode:\n\
\tEncrypts the provided message with the provided key (or seed) byte by byte. Required flags:\n\
\t\t-m (message file)\n\t\t-k (key file)\n\t\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data. Incompatible with the seed option.\n\
\t-s\n\t\tIndicate that the key is to be used as a seed for a ChaCha cipher.\n\
\t\tThis will disable the delete option.\n\
\t\tThe key can be up to 320 bits long (40 bytes)\n\
\t\tWARNING: This can significantly reduce the randomness of the key data, making the system less secure.\n\
\t\tThis should be used only if absolute security is unnecessary.\n";
            break;
        case BYTE:
            std::cout << "BYTE mode requires these flags:\n\
\t-m (message file)\n\t-k (key file)\n\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        complain(UNSET);
        return 1;
    }
    int i = 2; // current position from 0 to argc in argv
    char* message_name = {0};
    char* key_name = {0};
    char* out_name = {0};
    Mode mode;
    bool del = false; // save key data by default
    bool seed = false; // do not read key as a seed by default
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
            if (argv[i][1] == 'm' && argc > i+1) {
                ++i;
                message_name = argv[i];
            } else if (argv[i][1] == 'k' && argc > i+1) {
                ++i;
                key_name = argv[i];
            } else if (argv[i][1] == 'o' && argc > i+1) {
                ++i;
                out_name = argv[i];
            } else if (argv[i][1] == 's') {
                seed = true;
            } else if (argv[i][1] == 'd') {
                del = true;
            } else {
                std::cout << "Unknown flag or option: -" << argv[i][1] << "\n";
                return 1;
            }
        }
        ++i;
    }

    if (mode == BYTE) {
        if (message_name == 0 || key_name == 0 || out_name == 0) {
            complain(BYTE);
            return 1;
        }
        if (!seed) {
            return unseeded_byte(message_name, key_name, out_name, del);
        } else {
            return seeded_byte(message_name, key_name, out_name);
        }
    }
}
