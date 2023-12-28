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

#include "Method.hxx"
#include "../../Window.hxx"
#include "../../Application.hxx"
#include "../../Exec/Exec.hxx"

#include <iostream>
#include <filesystem>

Method::Method(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("clean_install_radio", clean_install_radio);
    builder->get_widget("manual_partition_radio", manual_partition_radio);
    builder->get_widget("selected_partition_radio", selected_partition_radio);

    builder->get_widget("storage_device_combo", storage_device_combo);
    builder->get_widget("boot_disk_combo", boot_disk_combo);
    builder->get_widget("selected_partition_combo", selected_partiton_combo);

    builder->get_widget("system_type_label", system_type_label);
    builder->get_widget("reload_partition_button", reload_partition_button);

    clean_install_radio->signal_toggled().connect(sigc::mem_fun(*this, &Method::on_clean_install));
    manual_partition_radio->signal_toggled().connect(sigc::mem_fun(*this, &Method::on_manual_partiton));
    selected_partition_radio->signal_toggled().connect(sigc::mem_fun(*this, &Method::on_selected_partiton));
    
    boot_disk_combo->signal_changed().connect(sigc::mem_fun(*this, &Method::on_boot_disk_changed));
    storage_device_combo->signal_changed().connect(sigc::mem_fun(*this, &Method::on_storage_device_changed));

    selected_partiton_combo->signal_changed().connect(sigc::mem_fun(*this, &Method::on_selected_partition_combo_changed));
    selected_partiton_combo->set_sensitive(false);

    reload_partition_button->signal_clicked().connect(sigc::mem_fun(*this, &Method::reload_partitions));

}

void Method::prepare(Gtk::Window *base) {
    Application::global->is_efi = std::filesystem::exists("/sys/firmware/efi");
    Application::global->window->get_nth_page(Application::DISK_PAGE)->hide();
    
    system_type_label->set_label(
        Application::global->is_efi
            ? "UEFI"
            : "BIOS"
    );
    
    reload_partitions();
    Application::global->window->set_page_complete(*this, false);
}


void Method::on_clean_install() {
    Application::global->window->get_nth_page(Application::DISK_PAGE)->hide();
    Application::global->window->set_page_complete(*this, true);
    Application::global->clean_install = true;
    selected_partiton_combo->set_sensitive(false);
}

void Method::on_selected_partiton() {
    Application::global->window->get_nth_page(Application::DISK_PAGE)->hide();
    Application::global->window->set_page_complete(*this, true);
    Application::global->clean_install = false;
    selected_partiton_combo->set_sensitive(true);
    on_selected_partition_combo_changed();
}
    
void Method::on_manual_partiton() {
    Application::global->window->get_nth_page(Application::DISK_PAGE)->show();
    Application::global->window->set_page_complete(*this, true);
    Application::global->clean_install = false;
    selected_partiton_combo->set_sensitive(false);
}

void Method::on_boot_disk_changed() {
    auto selected = boot_disk_combo->get_active_text();
    if (Application::global->is_efi) {
        Application::global->efi_partition = selected;
    } else {
        Application::global->boot_drive = selected;
    }
    
}

void Method::on_storage_device_changed() {
    Application::global->drive = storage_device_combo->get_active_text();

}

void Method::updated_block_devices() {
    auto [status, output] = Exec("lsblk", {"-J", "-O"}).output();
    if (status != 0) {
        std::cerr << "ERROR: " << output << std::endl;
        return;
    }

    try {
        block_devices = nlohmann::json::parse(output);
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
        return;
    }
}

void Method::reload_partitions() {
    storage_device_combo->remove_all();
    boot_disk_combo->remove_all();
    updated_block_devices();
    
    for (auto const &block: block_devices["blockdevices"]) {
        storage_device_combo->append(block["path"].get<std::string>());
        if (!Application::global->is_efi) {
            boot_disk_combo->append(block["path"].get<std::string>());
        }
    }

    if (Application::global->is_efi) {
        for (auto const &block: block_devices["blockdevices"]) {
            if (block.contains("children")) {
                for (auto const& child : block["children"]) {
                    if (child["parttype"] == "c12a7328-f81f-11d2-ba4b-00a0c93ec93b") {
                        boot_disk_combo->append(child["path"].get<std::string>());
                    }
                }
            }
        }
    }

      for (auto const &block: block_devices["blockdevices"]) {
        if (block.contains("children")) {
            for (auto const& child : block["children"]) {
                selected_partiton_combo->append(child["path"].get<std::string>());
            }
        }
    }

    if (!boot_disk_combo->get_cells().empty())
        boot_disk_combo->set_active(0);

    if (!storage_device_combo->get_cells().empty())
        storage_device_combo->set_active(0);
}

void Method::on_selected_partition_combo_changed() {
    Application::global->partition = selected_partiton_combo->get_active_text();
    if (Application::global->is_efi) {
        Application::global->efi_partition = boot_disk_combo->get_active_text();
    } else {
        Application::global->boot_drive = boot_disk_combo->get_active_text();
    }
}