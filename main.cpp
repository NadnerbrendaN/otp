/*
 * Copyright (C) 2025 NadnerbrendaN <albertdock@duck.com>
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
*/

#include <fstream>
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
    int i = 1;
    std::ifstream message_file; // a file input stream to hold the message
    std::fstream key_file; // a file input/output stream to use the key
    std::ofstream crypt_file; // a file output stream to write the cyphertext
    while (i < argc) { // loop until out of inputs
        if (argv[i][0] == '-' && argv[i][1] == 'm' && argc > i+1) { // message file
            ++i;
            message_file.open(argv[i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'k' && argc > i+1) { // key file
            ++i;
            key_file.open(argv[i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'o' && argc > i+1) { // cyphertext file
            ++i;
            crypt_file.open(argv[i]);
        }
        ++i;
    }
    if (!message_file.is_open() || !key_file.is_open() || !crypt_file.is_open()) {
        // complain if any files aren't open
        throw std::invalid_argument("Incorrect arguments. Format should be (in any order):\n\
                -m (message file) -k (key file) -o (output file)");
    }
}
