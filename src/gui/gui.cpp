/*
 * Copyright (C) 2026 NadnerbrendaN <albertdock@duck.com>
 *
 * A GTK GUI that allows users to encrypt and decrypt files using one time pads or a stream
 * cipher
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 *  If you did not recieve a copy of the MPL-2.0 with this source code, please contact the author
 *  to report the issue, and visit https://www.mozilla.org/media/MPL/2.0/index.f75d2927d3c1.txt
 *  to obtain a copy of the license text.
 */

#include "window.hpp"

OtpWindow::OtpWindow()
{
  set_title("otp");
  set_default_size(640, 480);
}

int main(int argc, char** argv)
{
  auto app = Gtk::Application::create("org.nadnerbrendan.otp");

  return app->make_window_and_run<OtpWindow>(argc, argv);
}
