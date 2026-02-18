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
#include "../otp.hpp"

#include <fstream>
#include <iostream>

OtpWindow::OtpWindow()
:
    button_run("Run"),
    button_key("Choose key file"),
    button_mess("Choose message file"),
    label_id("otp is an encryption system\nCopyright (C) 2026 NadnerbrendaN\nLicensed under the MPL 2.0"),
    label_delete("Delete used key data"),
    label_seed("Key is a seed"),
    label_file("No key file chosen"),
    label_out("Name the output file:"),
    label_mess("No message file chosen")
{
    set_title("otp");
    set_default_size(640, 480);

    key_file_name = "";
    message_file_name = "";

    root_grid.set_margin(8);
    set_child(root_grid);

    root_grid.attach(left_grid, 0,0);
    root_grid.attach(right_grid, 2,0);
    left_grid.set_valign(Gtk::Align::FILL);
    right_grid.set_margin_start(8);
    right_grid.set_halign(Gtk::Align::FILL);

    left_grid.attach(button_run, 0,0);
    button_run.set_margin_bottom(16);
    button_run.set_margin_top(24);
    button_run.set_halign(Gtk::Align::CENTER);
    button_run.set_valign(Gtk::Align::CENTER);
    button_run.set_size_request(128,48);
    button_run.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &OtpWindow::run_encrypt)));
    left_grid.attach(label_out, 0,1);
    left_grid.attach(text_out_name, 0,2);
    text_out_name.set_margin_bottom(8);
    text_out_name.set_margin_end(8);
    left_grid.attach(button_mess, 0,3);
    button_mess.set_margin_bottom(8);
    button_mess.set_margin_end(8);
    button_mess.set_size_request(128, 48);
    button_mess.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &OtpWindow::on_mess_button)));
    left_grid.attach(label_mess, 0,4);
    label_mess.set_max_width_chars(16);
    label_mess.set_wrap(true);

    root_grid.attach(scrollbox, 1,0, 1,3);
    scrollbox.set_child(textview);
    scrollbox.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    scrollbox.set_expand();
    textview.set_top_margin(8);
    textview.set_bottom_margin(8);
    textview.set_left_margin(8);
    textview.set_right_margin(8);
    textview.set_wrap_mode(Gtk::WrapMode::WORD_CHAR);
    textview.set_size_request(256, 128);

    root_grid.attach(label_id, 2,2);
    label_id.set_justify(Gtk::Justification::CENTER);
    label_id.set_margin_start(8);
    label_id.set_valign(Gtk::Align::END);
    root_grid.attach(label_status, 0,2);
    label_status.set_valign(Gtk::Align::END);
    label_status.set_markup("<span size='x-large'>Not active</span>");

    right_grid.attach(button_key, 0,0, 2,1);
    button_key.set_margin_top(8);
    button_key.set_margin_bottom(16);
    button_key.set_halign(Gtk::Align::CENTER);
    button_key.set_valign(Gtk::Align::CENTER);
    button_key.set_size_request(128, 48);
    button_key.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &OtpWindow::on_key_button)));
    right_grid.attach(switch_delete, 0,1);
    switch_delete.set_margin_end(8);
    switch_delete.set_margin_bottom(8);
    switch_delete.set_halign(Gtk::Align::END);
    switch_delete.set_size_request(48, 24);
    right_grid.attach(label_delete, 1,1);
    label_delete.set_halign(Gtk::Align::START);
    right_grid.attach(switch_seed, 0,2);
    switch_seed.set_margin_end(8);
    switch_seed.set_margin_bottom(8);
    switch_seed.set_halign(Gtk::Align::END);
    switch_seed.set_size_request(48, 24);
    right_grid.attach(label_seed, 1,2);
    label_seed.set_halign(Gtk::Align::START);
    right_grid.attach(label_file, 0,3, 2,1);
    label_file.set_margin_top(16);
    label_file.set_max_width_chars(30);
    label_file.set_wrap(true);
    label_file.set_size_request(258);
}

void OtpWindow::on_key_button() {
    auto dialogue = Gtk::FileDialog::create();
    dialogue->open(sigc::bind(sigc::mem_fun(*this, &OtpWindow::on_key_finish), dialogue));
}

void OtpWindow::on_key_finish(Glib::RefPtr<Gio::AsyncResult>& result,
        Glib::RefPtr<Gtk::FileDialog>& dialogue) {
    try {
        auto file = dialogue->open_finish(result);
        
        key_file_name = file->get_path();
        label_file.set_label("Key file: " + key_file_name);
    } catch (const Gtk::DialogError& err) {
        std::cout << "Unspecified file. " << err.what() << "\n";
    } catch (const Glib::Error& err) {
        std::cout << "Something went wrong: " << err.what() << "\n";
    }
}

void OtpWindow::on_mess_button() {
    auto dialogue = Gtk::FileDialog::create();
    dialogue->open(sigc::bind(sigc::mem_fun(*this, &OtpWindow::on_mess_finish), dialogue));
}

void OtpWindow::on_mess_finish(Glib::RefPtr<Gio::AsyncResult>& result,
        Glib::RefPtr<Gtk::FileDialog>& dialogue) {
    try {
        auto file = dialogue->open_finish(result);
        
        message_file_name = file->get_path();
        label_mess.set_label("Message file: " + message_file_name);
    } catch (const Gtk::DialogError& err) {
        std::cout << "Unspecified file. " << err.what() << "\n";
    } catch (const Glib::Error& err) {
        std::cout << "Something went wrong: " << err.what() << "\n";
    }
}

void OtpWindow::run_encrypt() {
    label_status.set_markup("<span size='x-large'>Processing...</span>");
    std::string mess_name = ".message.temp";
    Glib::ustring out_name = text_out_name.get_buffer()->get_text();
    if (message_file_name != "") {
        mess_name = message_file_name;
    } else {
        std::ofstream write_message(mess_name);
        Glib::ustring text = textview.get_buffer()->get_text();
        size_t text_size = text.size();
        for (int i = 0; i < text_size; ++i) {
            write_message.put(text[i]);
        }
        write_message.close();
    }
    label_status.set_markup("<span size='x-large'>Encrypting...</span>");
    if (switch_seed.get_active()) {
        seeded_byte(mess_name.data(), key_file_name.data(), out_name.data());
    } else {
        unseeded_byte(mess_name.data(), key_file_name.data(), out_name.data(), switch_delete.get_active());
    }
    std::remove(".message.temp");
    label_status.set_markup("<span size='x-large'>Not active</span>");
}
