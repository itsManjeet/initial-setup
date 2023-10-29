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

#ifndef INITIAL_SETUP_CONFIRM_HXX
#define INITIAL_SETUP_CONFIRM_HXX


#include "../Page.hxx"
#include "ConfigColumn.hxx"

#include <gtkmm.h>

class Confirm : public Page {
private:
    Gtk::TreeView *confirm_list_view;
    ConfigColumn config;
    Glib::RefPtr<Gtk::ListStore> ref_tree_model;

public:
    Confirm(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder);

    void prepare(Gtk::Window *window);
};

#endif //INITIAL_SETUP_CONFIRM_HXX
