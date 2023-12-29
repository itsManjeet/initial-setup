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

#include "Confirm.hxx"

#include "../../Window.hxx"
#include "../../Application.hxx"
#include <iostream>

Confirm::Confirm(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("confirm_list_view", confirm_list_view);
    ref_tree_model = Gtk::ListStore::create(config);

    confirm_list_view->set_model(ref_tree_model);
    confirm_list_view->append_column("Name", config.name);
    confirm_list_view->append_column("Value", config.value);

}

void Confirm::prepare(Gtk::Window *base) {
    if (!ref_tree_model->children().empty()) {
        ref_tree_model->clear();
    }

    auto row = *(ref_tree_model->append());
    row[config.name] = "Mode";
    row[config.value] = Application::global->mode == Application::Mode::Installer ? "Installer" : "First Boot";

    switch (Application::global->mode) {
        case Application::Mode::Testing:
        case Application::Mode::Installer: {
            row = *(ref_tree_model->append());
            row[config.name] = "Installation Method";
            row[config.value] = Application::global->clean_install ? "Clean Install" : "Manual Partitioning";

            row = *(ref_tree_model->append());
            row[config.name] = "System Type";
            row[config.value] = Application::global->is_efi ? "UEFI" : "Boot Legacy";

            if (Application::global->is_efi) {
                row = *(ref_tree_model->append());
                row[config.name] = "EFI Partition";
                row[config.value] = Application::global->efi_partition;
            } else {
                row = *(ref_tree_model->append());
                row[config.name] = "BootLoader Device";
                row[config.value] = Application::global->boot_drive;
            }

            if (Application::global->clean_install) {
                row = *(ref_tree_model->append());
                row[config.name] = "Storage Drive";
                row[config.value] = Application::global->drive;
            } else {
                row = *(ref_tree_model->append());
                row[config.name] = "Root Partition";
                row[config.value] = Application::global->partition;
            }
        }
            break;
        case Application::Mode::InitialSetup: {
            row = *(ref_tree_model->append());
            row[config.name] = "User Name";
            row[config.value] = Application::global->username;

            row = *(ref_tree_model->append());
            row[config.name] = "Timezone";
            row[config.value] = Application::global->timezone;

            row = *(ref_tree_model->append());
            row[config.name] = "Auto Login";
            row[config.value] = Application::global->autologin ? "true" : "false";

            row = *(ref_tree_model->append());
            row[config.name] = "Update Root password";
            row[config.value] = Application::global->update_root_password ? "true" : "false";
        }
            break;
    }


    Application::global->window->set_page_complete(*this, true);


}
