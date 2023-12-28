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

#ifndef INITIAL_SETUP_METHOD_HXX
#define INITIAL_SETUP_METHOD_HXX

#include "../Page.hxx"
#include "../../json.hxx"

class Method : public Page {
private:
    Gtk::RadioButton *clean_install_radio{nullptr},
                     *selected_partition_radio{nullptr},
                     *manual_partition_radio{nullptr};
    Gtk::ComboBoxText *boot_disk_combo{nullptr},
                      *storage_device_combo{nullptr},
                      *selected_partiton_combo{nullptr};
    Gtk::Label* system_type_label{nullptr};
    Gtk::Button* reload_partition_button{nullptr};

    nlohmann::json block_devices;

    void updated_block_devices();

    void reload_partitions();

protected:
    void on_clean_install();

    void on_selected_partiton();
    
    void on_manual_partiton();

    void on_boot_disk_changed();

    void on_storage_device_changed();

    void on_selected_partition_combo_changed();
    
public:
    Method(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder);

    void prepare(Gtk::Window *window);
};


#endif //INITIAL_SETUP_WELCOME_HXX
