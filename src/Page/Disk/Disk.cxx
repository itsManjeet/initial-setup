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

#include "Disk.hxx"
#include "../../Exec/Exec.hxx"
#include "../../json.hxx"
#include "../../Window.hxx"
#include "../../Application.hxx"
#include <iostream>

Disk::Disk(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("disk_list_view", disk_list_view);
    builder->get_widget("disk_refresh_button", disk_refresh_button);
    builder->get_widget("disk_edit_button", disk_edit_button);
    ref_disk_model = Gtk::ListStore::create(disks);

    disk_list_view->set_model(ref_disk_model);
    disk_list_view->append_column("Path", disks.path);
    disk_list_view->append_column("Name", disks.name);
    disk_list_view->append_column("Type", disks.type);
    disk_list_view->append_column("Size", disks.size);
    disk_list_view->append_column("Hint", disks.hint);

    disk_list_view->signal_row_activated()
            .connect(sigc::mem_fun(*this, &Disk::on_treeview_row_activated));

    disk_refresh_button->signal_clicked()
            .connect(sigc::mem_fun(*this, &Disk::on_disk_refresh_button_clicked));
    disk_edit_button->signal_clicked()
            .connect(sigc::mem_fun(*this, &Disk::on_disk_edit_button_clicked));
}

void Disk::prepare(Gtk::Window *base) {
    if (!ref_disk_model->children().empty()) {
        ref_disk_model->clear();
    }
    Application::global->window->set_page_complete(*this, false);
    Application::global->is_efi = std::filesystem::exists("/sys/firmware/efi");

    // if (Application::global->mode == Application::Mode::Testing) {
    //     auto row = *(ref_disk_model->append());
    //     row[disks.path] = "/dev/sda1";
    //     row[disks.size] = "10G";
    //     row[disks.type] = "ext4";
    //     row[disks.name] = "RLXOS";

    //     row = *(ref_disk_model->append());
    //     row[disks.path] = "/dev/sda2";
    //     row[disks.size] = "100G";
    //     row[disks.type] = "btrfs";

    //     return;
    // }

    auto [status, output] = Exec("lsblk", {"-J", "-O"}).output();
    if (status != 0) {
        std::cerr << "ERROR: " << output << std::endl;
        return;
    }

    nlohmann::json data;
    try {
        data = nlohmann::json::parse(output);
    } catch (const std::exception &error) {
        std::cerr << "ERROR: " << error.what() << std::endl;
        return;
    }

    for (auto const &block: data["blockdevices"]) {
        if (block.contains("children")) {
            for (auto const &child: block["children"]) {
                if (Application::global->is_efi                 && 
                    Application::global->efi_partition.empty()  &&
                    !child["rm"].get<bool>()) {
                    if (child["parttype"] == "c12a7328-f81f-11d2-ba4b-00a0c93ec93b") {
                        Application::global->efi_partition = child["path"].get<std::string>();
                    }
                }
                auto row = *(ref_disk_model->append());
                row[disks.path] = child["path"].template get<std::string>();
                row[disks.size] = child["size"].template get<std::string>();
                if (child.contains("fstype") && child["fstype"].is_string()) {
                    row[disks.type] = child["fstype"].template get<std::string>();
                }
                if (child.contains("label") && child["label"].is_string()) {
                    row[disks.name] = child["label"].get<std::string>();
                }
                row[disks.hint] = child["parttypename"].get<std::string>();
            }
        }

    }
}

void Disk::on_treeview_row_activated(const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column) {
    const auto iter = ref_disk_model->get_iter(path);
    if (iter) {
        const auto row = *iter;
        Application::global->partition = (Glib::ustring) row[disks.path];
        Application::global->window->set_page_complete(*this, true);
    }
}

void Disk::on_disk_edit_button_clicked() {
    auto pid = fork();
    if (pid == 0) {
        execl("/usr/bin/gparted", "/usr/bin/gparted");
        exit(0);
    }
}

void Disk::on_disk_refresh_button_clicked() {
    prepare(nullptr);
}
