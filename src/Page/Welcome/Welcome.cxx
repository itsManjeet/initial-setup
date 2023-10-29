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

Welcome::Welcome(Page::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Page(object) {
    builder->get_widget("welcome_message", message);
}

void Welcome::prepare(Gtk::Window *base) {
    switch (Application::global->mode) {
        case Application::Mode::InitialSetup:
            message->set_text("Follow the guide to complete the first boot setup");
            break;
        case Application::Mode::Installer:
            message->set_text("Follow the guide to install rlxos onto your system");
            break;
    }
}
