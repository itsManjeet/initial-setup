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
    builder->get_widget("summary_scroll", summary_scroll);

    builder->get_widget("summary_status", status_label);
    builder->get_widget("summary_icon", status_icon);

}

void Summary::prepare(Gtk::Window *base) {
    if (Application::global->failed) {
        status_icon->set_from_icon_name("gtk-cancel", Gtk::ICON_SIZE_DIALOG);
        status_label->set_text("Installation Failed");
        summary_view->get_buffer()->set_text(Application::global->error_message);
    } else {
        status_icon->set_from_icon_name("gtk-apply", Gtk::ICON_SIZE_DIALOG);
        status_label->set_text("Success");
        status_label->set_vexpand();
        summary_scroll->hide();
    }

}

