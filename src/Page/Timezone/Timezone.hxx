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

#ifndef INITIAL_SETUP_TIMEZONE_HXX
#define INITIAL_SETUP_TIMEZONE_HXX

#include "../Page.hxx"

class TimezoneColumn : public Gtk::TreeModel::ColumnRecord {
public:
    TimezoneColumn() {
        add(name);
    }

    Gtk::TreeModelColumn<Glib::ustring> name;
};

class Timezone : public Page {
private:
    Gtk::Entry* timezone_entry{nullptr};
    Gtk::CheckButton* utc_check{nullptr};
    Glib::RefPtr<Gtk::ListStore> timezone_model;
    Glib::RefPtr<Gtk::TreeModelFilter> filter;
    TimezoneColumn column;

protected:
    void on_utc_selected();

    bool on_timezone_selected(const Gtk::TreeModel::const_iterator& iter) const;

    bool compare_fun(const Glib::ustring& key, const Gtk::TreeModel::iterator& iter);
public:
    Timezone(Page::BaseObjectType* object, const Glib::RefPtr<Gtk::Builder>& builder);

    void prepare(Gtk::Window* window);
};

#endif // INITIAL_SETUP_TIMEZONE_HXX
