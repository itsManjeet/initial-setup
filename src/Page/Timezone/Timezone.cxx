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
#include "../../Application.hxx"
#include "../../Window.hxx"

#include <fstream>

// TODO: a much better way to handle timezone

Timezone::Timezone(Page::BaseObjectType* object, const Glib::RefPtr<Gtk::Builder>& builder)
    : Page(object) {
    builder->get_widget("timezone_entry", timezone_entry);
    builder->get_widget("utc_check", utc_check);

    timezone_model = Gtk::ListStore::create(column);

    auto completion = Gtk::EntryCompletion::create();
    completion->set_model(timezone_model);
    completion->set_text_column(0);
    completion->set_match_func(sigc::mem_fun(*this, &Timezone::compare_fun));

    timezone_entry->set_completion(completion);

    auto _ = completion->signal_match_selected().connect(sigc::mem_fun(*this, &Timezone::on_timezone_selected));

    _ = utc_check->signal_toggled().connect(sigc::mem_fun(*this, &Timezone::on_utc_selected));
    utc_check->set_active(true);
}

void Timezone::prepare(Gtk::Window* base) {
    std::ifstream reader("/usr/share/zoneinfo/tzdata.zi");
    if (!reader.good()) {
        utc_check->set_active(true);
        utc_check->set_sensitive(false);
        timezone_entry->set_sensitive(false);
        Application::global->window->set_page_complete(*this, true);
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
    timezone_model->clear();
    for (auto const& t : timezones) {
        auto row = *timezone_model->append();
        row[column.name] = t;
    }

    Application::global->window->set_page_complete(*this, true);
}

void Timezone::on_utc_selected() {
    timezone_entry->set_sensitive(!utc_check->get_active());
}

bool Timezone::on_timezone_selected(const Gtk::TreeModel::const_iterator& iter) const {
    if (iter) {
        auto row = *iter;
        Application::global->timezone = static_cast<Glib::ustring>(row[column.name]);
    }
    return true;
}

bool Timezone::compare_fun(const Glib::ustring& key, const Gtk::TreeModel::iterator& iter) {
    auto row = *iter;
    auto timezone = static_cast<Glib::ustring>(row[column.name]);
    auto l_key = key.lowercase();
    auto l_timezone = timezone.lowercase();
    return l_timezone.find(l_key) != Glib::ustring::npos;
}
