/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A header declaring window constants and widgets and stuff
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#ifndef OTP_WINDOW_HEADER
#define OTP_WINDOW_HEADER

#include <gtkmm.h>

class OtpWindow : public Gtk::Window
{
public:
  OtpWindow();

private:
  Gtk::Grid root_grid;
};

#endif
