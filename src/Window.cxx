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

#include "Window.hxx"
#include "Application.hxx"

#include "config.hxx"
#include <iostream>


Window *Window::create() {
    auto builder = Gtk::Builder::create_from_resource(APPLICATION_PREFIX "ui/window.glade");
    Window *window = nullptr;
    builder->get_widget_derived("window", window);
    if (window == nullptr) {
        throw std::runtime_error("No 'window' object in ui/window.glade");
    }


    return window;
}

Window::Window(Gtk::Assistant::BaseObjectType *object, const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::Assistant(object),
          dispatcher(),
          worker_(),
          worker_thread_{nullptr} {
    builder->get_widget_derived("welcome_page", welcome_page);
    builder->get_widget_derived("method_page", method_page);
    builder->get_widget_derived("disk_page", disk_page);
    builder->get_widget_derived("confirm_page", confirm_page);
    builder->get_widget_derived("user_page", user_page);
    builder->get_widget_derived("timezone_page", timezone_page);
    builder->get_widget_derived("summary_page", summary_page);

    builder->get_widget("progress_spinner", progress_spinner);
    builder->get_widget("progress_message", progress_message);
    builder->get_widget("progress_view", progress_view);

    progress_buffer = progress_view->get_buffer();
    progress_buffer->create_mark("last_line", progress_buffer->end(), true);

    dispatcher.connect(sigc::mem_fun(*this, &Window::on_notification_from_worker_thread));
}

void Window::on_apply() {
    this->commit();
    if (worker_thread_) {
        progress_message->set_label("can't start a worker thread, already started");
        progress_spinner->stop();
    } else {
        progress_spinner->start();
        worker_thread_ = new std::thread([this]() {
            worker_.start(this);
        });
    }
}

void Window::on_cancel() {
    std::cout << "cancelling" << std::endl;
    this->hide();
}

void Window::on_close() {
    if (Application::global->mode == Application::Mode::Installer) {
        system("reboot");
    } else {
        system("xfce4-session-logout --logout");
    }

    this->hide();
}


void Window::on_prepare(Gtk::Widget *widget) {
    auto page = dynamic_cast<Page *>(widget);
    if (page == nullptr) return;

    page->prepare(this);
}

void Window::on_notification_from_worker_thread() {
    if (worker_thread_ && worker_.has_stopped()) {
        if (worker_thread_->joinable()) {
            worker_thread_->join();
        }
        delete worker_thread_;
        worker_thread_ = nullptr;
        next_page();
    }

    double progress;
    Glib::ustring message;
    worker_.get_data(&progress, &message);

    if (auto idx = message.find_last_of("::"); idx != std::string::npos) {
        auto m = message.substr(idx + 2, message.find('\n', idx + 2) - (idx + 2));
        if (m.length() > 25) {
            m = m.substr(0, 23) + "...";
        }
        progress_message->set_text(m);
    }

    if (message != progress_buffer->get_text()) {
        progress_buffer->set_text(message);
        auto iter = progress_buffer->end();
        iter.set_line_offset(0);
        auto mark = progress_buffer->get_mark("last_line");
        progress_buffer->move_mark(mark, iter);
    }
}

void Window::notify() {
    dispatcher.emit();
}

Window::~Window() = default;
