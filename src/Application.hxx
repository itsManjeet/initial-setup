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

#ifndef INITIAL_SETUP_APPLICATION_HXX
#define INITIAL_SETUP_APPLICATION_HXX

#include <gtkmm/application.h>
#include "Window.hxx"


class Application : public Gtk::Application {
public:
    enum class Mode {
        Installer,
        InitialSetup,
    };

    struct Configuration {
        bool is_efi;
        Mode mode;
        Window *window;
        std::string username;
        std::string password;


        std::string partition;
        std::string efi_partition;

        bool failed;
        std::string error_message;
    };

    static Configuration *global;
private:
    Application();

    Window *create_window();

public:
    ~Application() override;

    static Glib::RefPtr<Application> create();

    void on_activate() override;

    void on_startup() override;

    void on_hide_window(Gtk::Window *window);
};


#endif //INITIAL_SETUP_APPLICATION_HXX
