/* 
 * Copyright (c) 2023 rlxos.
 *
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Created by
 *    - Manjeet Singh <itsmanjeet1998@gmail.com> on 29/10/23.
 */

#include "User.hxx"

#include "../../Window.hxx"
#include "../../Application.hxx"

User::User(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("user_entry", user_entry);
    builder->get_widget("password_entry", password_entry);
    builder->get_widget("user_superuser_password_check", superuser_password_check);
    builder->get_widget("user_autologin_check", autologin_check);

    user_entry->signal_changed()
            .connect(sigc::mem_fun(*this, &User::on_entry_changed));
    password_entry->signal_changed()
            .connect(sigc::mem_fun(*this, &User::on_entry_changed));

    password_entry->signal_icon_press()
            .connect(sigc::mem_fun(*this, &User::on_password_view));

    superuser_password_check->signal_clicked()
            .connect(sigc::mem_fun(*this, &User::on_superuser_password_check));
    autologin_check->signal_clicked()
            .connect(sigc::mem_fun(*this, &User::on_autologin_check));
}

void User::prepare(Gtk::Window *base) {
    Application::global->autologin = autologin_check->get_active();
    Application::global->update_root_password = superuser_password_check->get_active();
}

void User::on_entry_changed() {
    std::string username = user_entry->get_text();
    for (char &i: username) {
        if (std::isspace(i) || i == '-') i = '_';
        else if (std::isupper(i)) i = std::tolower(i);
        else if (!std::isalpha(i)) i = 0;

    }
    user_entry->set_text(username);
    if (!username.empty() && !password_entry->get_text().empty()) {
        Application::global->window->set_page_complete(
                *this,
                true);
        Application::global->username = username;
        Application::global->password = password_entry->get_text();
    } else {
        Application::global->window->set_page_complete(
                *this,
                false);
    }


}

void User::on_superuser_password_check() {
    Application::global->update_root_password = superuser_password_check->get_active();
}

void User::on_autologin_check() {
    Application::global->autologin = autologin_check->get_active();
}

void User::on_password_view(Gtk::EntryIconPosition, const GdkEventButton *event) {
    if (password_entry->get_visibility()) {
        password_entry->set_visibility(false);
        password_entry->set_icon_from_icon_name("view-reveal-symbolic", Gtk::EntryIconPosition::ENTRY_ICON_SECONDARY);
    } else {
        password_entry->set_visibility(true);
        password_entry->set_icon_from_icon_name("view-conceal-symbolic", Gtk::EntryIconPosition::ENTRY_ICON_SECONDARY);
    }
}
