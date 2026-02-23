/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A header to make sure the lsp doesn't yell at me about undeclared symbols that I'm going to link eventually
 * (make sure you link the right files)
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not receive a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include <cstdint>

void chacha(std::uint32_t* out, std::uint32_t* in);
