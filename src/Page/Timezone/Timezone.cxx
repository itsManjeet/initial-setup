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

#include "Timezone.hxx"
#include "../../Window.hxx"
#include "../../Application.hxx"

#include <fstream>

// TODO: a much better way to handle timezone

Timezone::Timezone(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {

    builder->get_widget("timezone_combo", timezone_combo); 
    builder->get_widget("utc_check", utc_check);

    utc_check->signal_toggled().connect(sigc::mem_fun(*this, &Timezone::on_utc_selected));
    timezone_combo->signal_changed().connect(sigc::mem_fun(*this, &Timezone::set_timezone));
    utc_check->set_active(true);
}

void Timezone::prepare(Gtk::Window *base) {
    std::ifstream reader("/usr/share/zoneinfo/tzdata.zi");
    if (!reader.good()) {
        utc_check->set_active(true);
        utc_check->set_sensitive(false);
        timezone_combo->set_sensitive(false);
        set_timezone();
        return;
    }

    std::vector<std::string> timezones;
    for (std::string line; std::getline(reader, line);) {
        if (!line.empty() && line.starts_with("Z")) {
            std::stringstream ss(line);
            std::string Z, timezone;
            ss >> Z >> timezone;

            timezones.push_back(timezone);
        }
    }
    std::sort(timezones.begin(), timezones.end());
    for(auto const& t : timezones) timezone_combo->append(t);

    Application::global->window->set_page_complete(*this, true);
}

void Timezone::on_utc_selected() {
    timezone_combo->set_sensitive(!utc_check->get_active());
    
}

void Timezone::set_timezone() {
    if (utc_check->get_active()) {
        Application::global->timezone = "UTC";
    } else {
        Application::global->timezone = timezone_combo->get_active_text();
    }
}