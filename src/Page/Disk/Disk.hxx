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

#ifndef INITIAL_SETUP_DISK_HXX
#define INITIAL_SETUP_DISK_HXX

#include "../Page.hxx"
#include "DiskColumn.hxx"

#include <gtkmm.h>

class Disk : public Page {
private:
    Gtk::TreeView *disk_list_view;
    Gtk::Button *disk_edit_button, *disk_refresh_button;
    DiskColumn disks;
    Glib::RefPtr<Gtk::ListStore> ref_disk_model;
protected:
    void on_treeview_row_activated(const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column);

    void on_disk_edit_button_clicked();

    void on_disk_refresh_button_clicked();

public:
    Disk(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder);

    void prepare(Gtk::Window *window);
};


#endif //INITIAL_SETUP_DISK_HXX
