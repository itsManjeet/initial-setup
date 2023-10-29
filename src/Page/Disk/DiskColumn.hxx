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

#ifndef INITIAL_SETUP_DISKCOLUMN_HXX
#define INITIAL_SETUP_DISKCOLUMN_HXX

#include <gtkmm.h>

class DiskColumn : public Gtk::TreeModel::ColumnRecord {
public:
    DiskColumn() {
        add(path);
        add(name);
        add(type);
        add(size);
        add(hint);
    }

    Gtk::TreeModelColumn<Glib::ustring> path;
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> type;
    Gtk::TreeModelColumn<Glib::ustring> hint;
    Gtk::TreeModelColumn<Glib::ustring> size;
};


#endif //INITIAL_SETUP_DISKCOLUMN_HXX
