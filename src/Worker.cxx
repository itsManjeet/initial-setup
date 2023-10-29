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

#include "Worker.hxx"
#include "Window.hxx"
#include "Application.hxx"

Worker::Worker()
        : mutex_(),
          has_stopped_{false},
          progress_{0.0},
          message_() {

}

void Worker::start(Window *caller) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        has_stopped_ = false;
        progress_ = 0.0;
        message_ = "";

        std::stringstream cmd;
        cmd << "env ";
        switch (Application::global->mode) {
            case Application::Mode::Installer: {
                cmd << " ISE_ROOT=" << Application::global->partition;
                if (Application::global->is_efi) {
                    cmd << " ISE_EFI=" << Application::global->efi_partition;
                    cmd << " ISE_IS_EFI=1";
                }

                cmd << " /usr/lib/initial-setup/installer.sh";
            }
                break;
            case Application::Mode::InitialSetup: {
                cmd << " ISE_USERNAME=" << Application::global->username;
                cmd << " ISE_PASSWORD=" << Application::global->password;
                cmd << " /usr/lib/initial-setup/first-boot.sh";

            }
                break;
        }

        cmd << " 2>&1";

        pipe_ = popen(cmd.str().c_str(), "r");
        if (pipe_ == nullptr) {
            has_stopped_ = true;
            progress_ = 1.0;
            Application::global->failed = true;
            Application::global->error_message = "failed to open pipe (" + std::string(strerror(errno)) + ")";
            caller->notify();
            return;
        }
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::array<char, 128> buffer{};
            if (fgets(buffer.data(), buffer.size(), pipe_) == nullptr) {
                break;
            }
            message_.append(buffer.data());
        }
        caller->notify();
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (int status = WEXITSTATUS(pclose(pipe_)); status != 0) {
            Application::global->error_message
                    = message_ +
                      ":: Failed with error code " + std::to_string(status) +
                      "\n";
            Application::global->failed = true;
        }

        has_stopped_ = true;
        progress_ = 1.0;
    }

    caller->notify();
}

bool Worker::has_stopped() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return has_stopped_;
}

void Worker::get_data(double *fraction_done, Glib::ustring *message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (fraction_done)
        *fraction_done = progress_;
    if (message)
        *message = message_;
}
