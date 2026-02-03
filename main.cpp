/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A simple CLI to that allows users to encrypt and decrypt files using one time pads and a
 * stream cipher.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <cstdio>
#include <fstream>
#include <iostream>
#include "otp.cpp"

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
\n\tGEN mode:\n\
\tGenerates keys or seeds based on the arguments specified. This is a work in progress. It doesn't function.\n\
\n\tBYTE mode:\n\
\tEncrypts the provided message with the provided key (or seed) byte by byte. Required flags:\n\
\t\t-m (message file)\n\t\t-k (key file)\n\t\t-o (output file)\n\nOptions:\n\
\t-d\n\t\tDelete used key data. Incompatible with the seed option.\n\
\t-e\n\t\tEncrypt the message, instead of the default decryption setting.\n\
\t-s\n\t\tIndicate that the key is to be used as a seed for a ChaCha cipher.\n\
\t\tThis will disable the delete option.\n\
\t\tWARNING: This can significantly reduce the randomness of the key data, making the system less secure.\n\
\t\tThis should be used only if absolute security is unnecessary.\n\
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
            } else if (argv[i][1] == 'e') { // encrypt option
                enc = true;
            } else if (argv[i][1] == 'd') { // delete option
                del = true;
            } else {
                std::cout << "Unknown flag or option: -" << argv[i][0] << "\n";
                return 2;
            }
        }
        ++i;
    }

    if (mode == BYTE) {
        if (message_name == 0 || key_name == 0 || out_name == 0) {
            complain(BYTE);
            return 3;
        }
        if (!seed) {
            return unseeded_byte(message_name, key_name, out_name, enc, del);
        } else {
            return seeded_byte(message_name, key_name, out_name, enc);
        }
    }
}
