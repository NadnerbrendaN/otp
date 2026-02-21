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

class OtpWindow : public Gtk::Window {
public:
  OtpWindow();
private:
  void on_out_button();
  void on_out_finish(Glib::RefPtr<Gio::AsyncResult>&, Glib::RefPtr<Gtk::FileDialog>&);
  void on_key_button();
  void on_key_finish(Glib::RefPtr<Gio::AsyncResult>&, Glib::RefPtr<Gtk::FileDialog>&);
  void on_mess_button();
  void on_mess_finish(Glib::RefPtr<Gio::AsyncResult>&, Glib::RefPtr<Gtk::FileDialog>&);
  void run_encrypt();

  std::string key_file_name, message_file_name, out_file_name;

  Gtk::Grid root_grid, left_grid, right_grid;
  Gtk::Button button_run, button_key, button_mess, button_out;
  Gtk::Switch switch_encrypt, switch_delete, switch_seed;
  Gtk::Label label_id, label_encrypt_left, label_encrypt_right, label_delete, label_seed, label_file,
      label_out, label_mess, label_status;
  Gtk::ScrolledWindow scrollbox;
  Gtk::TextView textview;
};

#endif
