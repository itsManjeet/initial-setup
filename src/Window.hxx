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
 *    - Manjeet Singh <itsmanjeet1998@gmail.com> on 28/10/23.
 */

#ifndef INITIAL_SETUP_WINDOW_HXX
#define INITIAL_SETUP_WINDOW_HXX

#include <gtkmm.h>
#include "Page/Welcome/Welcome.hxx"
#include "Page/Method/Method.hxx"
#include "Page/Disk/Disk.hxx"
#include "Page/Confirm/Confirm.hxx"
#include "Page/User/User.hxx"
#include "Page/Timezone/Timezone.hxx"
#include "Page/Summary/Summary.hxx"
#include "Worker.hxx"

class Window : public Gtk::Assistant {
private:
    Welcome *welcome_page;
    Method* method_page;
    User *user_page;
    Timezone *timezone_page;
    Disk *disk_page;
    Summary *summary_page;

    Gtk::Spinner *progress_spinner;
    Gtk::TextView *progress_view;
    Glib::RefPtr<Gtk::TextBuffer> progress_buffer;
    Gtk::Label *progress_message;
    Confirm *confirm_page;

    Glib::Dispatcher dispatcher;
    Worker worker_;
    std::thread *worker_thread_;

    void on_apply();

    void on_cancel();

    void on_close();

    void on_prepare(Gtk::Widget *widget);

    void on_notification_from_worker_thread();

public:
    Window(Gtk::Assistant::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder);

    ~Window() override;

    void notify();

    static Window *create();
};


#endif //INITIAL_SETUP_WINDOW_HXX
