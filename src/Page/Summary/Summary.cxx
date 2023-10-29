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

#include "Summary.hxx"
#include "../../Window.hxx"
#include "../../Application.hxx"

Summary::Summary(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("summary_view", summary_view);
    builder->get_widget("finish_button", finish_button);

    finish_button->signal_clicked().connect(sigc::mem_fun(*this, &Summary::on_finished));
}

void Summary::prepare(Gtk::Window *base) {
    std::string message;
    if (Application::global->failed) {
        message = "FAILED: " + Application::global->error_message;
    } else {
        message = "Success";
    }

    summary_view->get_buffer()->set_text(message);
    finish_button->set_label(
            Application::global->mode == Application::Mode::Installer
            ? "Reboot"
            : "Finalize"
    );
}

void Summary::on_finished() {
    if (Application::global->mode == Application::Mode::Installer) {
        system("reboot");
    } else {
        system("xfce4-session-logout");
    }
}
