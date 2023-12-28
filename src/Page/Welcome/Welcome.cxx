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

#include "Welcome.hxx"
#include "../../Window.hxx"
#include "../../Application.hxx"

#define INITIAL_SETUP_USER_ID 82

Welcome::Welcome(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("message_label", message_label);
}

void Welcome::prepare(Gtk::Window *base) {
    if (geteuid() != INITIAL_SETUP_USER_ID && Application::global->mode != Application::Mode::Testing) {
        Application::global->window->set_page_complete(*this, false);
        message_label->set_label(
                R"(You have already completed the Installation and
initial setup process, If you need any help please
reach out at our community groups and support email)");

        // TODO: Less hacky way to hide sidebar
        for (int i = 0; i < Application::global->window->get_n_pages(); i++) {
            Application::global->window->set_page_title(*Application::global->window->get_nth_page(i), "");
        }
    }
}
