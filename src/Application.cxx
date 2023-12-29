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

#include "Application.hxx"
#include "config.hxx"

#include <iostream>
#include <fstream>


Application::Configuration *Application::global = nullptr;

Application::Application() : Gtk::Application(APPLICATION_ID) {
    Application::global = new Application::Configuration{};
}

Application::~Application() {
    delete Application::global;
}

Glib::RefPtr<Application> Application::create() {
    return Glib::RefPtr<Application>(new Application());
}

Window *Application::create_window() {
    if (get_windows().empty()) {
        auto window = Window::create();
        add_window(*window);
    }
    auto window = get_windows().front();
    window->signal_hide().connect(sigc::bind(sigc::mem_fun(*this, &Application::on_hide_window), window));
    return static_cast<Window*>(window);
}

void Application::on_activate() {
    try {
        auto window = create_window();
        global->mode = Mode::InitialSetup;
        global->window = window;
        if (getenv("INITIAL_SETUP_EFI")) {
            global->efi_partition = getenv("INITIAL_SETUP_EFI");
        }

        std::ifstream cmdline("/proc/cmdline");
        if (cmdline.good()) {
            for (std::string line; cmdline >> line;) {
                if (line == "rd.live=1") {
                    global->mode = Mode::Installer;
                    break;
                }
            }
        }

        switch (global->mode) {
            case Mode::Installer:
                window->get_nth_page(USER_PAGE)->hide();
                window->get_nth_page(TIMEZONE_PAGE)->hide();
                break;
            case Mode::InitialSetup:
                // Hide installer Page
                window->get_nth_page(METHOD_PAGE)->hide();
                window->get_nth_page(DISK_PAGE)->hide();
                break;
        }
        cmdline.close();
        window->present();
    } catch (const Glib::Error &error) {
        std::cerr << "Application::on_activate(): " << error.what() << std::endl;
    } catch (const std::exception &error) {
        std::cerr << "Application::on_activate(): " << error.what() << std::endl;
    }
}

void Application::on_startup() {
    Gtk::Application::on_startup();
}

void Application::on_hide_window(Gtk::Window *window) {
    window->hide();
}
