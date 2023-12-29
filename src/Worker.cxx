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

#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>

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
        switch (Application::global->mode) {
            case Application::Mode::Installer: {                
                if (Application::global->clean_install) {
                    cmd << " ISE_CLEAN_INSTALL=1";
                    cmd << " ISE_DEVICE=" << Application::global->drive;
                } else {
                    cmd << " ISE_ROOT=" << Application::global->partition;
                }
                if (Application::global->is_efi) {
                    cmd << " ISE_EFI=" << Application::global->efi_partition;
                    cmd << " ISE_IS_EFI=1";
                } else {
                    cmd << " ISE_BOOT_DEVICE=" << Application::global->boot_drive;
                }

                cmd << " /usr/lib/initial-setup/installer.sh";
            }
                break;
            case Application::Mode::InitialSetup: {
                cmd << " ISE_USERNAME=" << Application::global->username;
                cmd << " ISE_PASSWORD=" << Application::global->password;
                cmd << " ISE_AUTOLOGIN=" << (Application::global->autologin ? "1" : "0");
                cmd << " ISE_TIMEZONE=" << Application::global->timezone;
                cmd << " ISE_UPDATE_ROOT_PASSWORD=" << (Application::global->update_root_password ? "1" : "0");
                cmd << " /usr/lib/initial-setup/first-boot.sh";

            }
                break;
            
            case Application::Mode::Testing: {
                cmd << "for i in 1 2 3 4 5 6 7 9 10 ; do echo PROCESS ${i}; sleep 1; done; sleep 5; exit 1";
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
        completed = false;
        fd_ = fileno(pipe_);
        int flags = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
        
        fds[0].fd = fd_;
        fds[0].events = POLLIN | POLLHUP | POLLERR;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::array<char, 128> buffer{};
            auto result = poll(fds, 1, 0);
            std::cout << "POLL: " << result << std::endl;
            switch (result) {
                case -1:
                    std::cout << "ERROR: select(failed)" << strerror(errno) << std::endl;
                    completed = true;
                    Application::global->failed = true;
                    break;

                case 0: {
                    int status;
                    auto pid = waitpid(-1, &status, WNOHANG);
                    if (pid == -1 || WIFEXITED(status)) {
                        completed = true;
                        Application::global->failed = WEXITSTATUS(status) != 0;
                    }
                }
                    break;
                
                default: {
                    if (fds[0].revents & POLLIN) {
                        if (fgets(buffer.data(), buffer.size(), pipe_) == nullptr) {
                            std::cout << "ERROR: failed to read data " << strerror(errno) << std::endl;
                        } else {
                            std::cout << "READING DATA" << std::endl;
                            message_.append(buffer.data());
                        }
                    } else if (fds[0].revents & POLLERR) {
                        completed = true;
                        Application::global->failed = true;
                        Application::global->error_message = message_ + "\npoll(error) " + std::string(strerror(errno));
                    } else if (fds[0].revents & POLLHUP) {
                        completed = true;
                    } else {
                        std::cout << "REVENT: " << fds[0].revents << std::endl;
                    }
                }
            }
            if (completed || Application::global->failed) {
                std::cout << "COMPLETED" << std::endl;
                break;
            }
        }
        caller->notify();
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        int status = WEXITSTATUS(pclose(pipe_));
        if (status != 0) {
            Application::global->failed = true;
            Application::global->error_message = message_ + "\nExit With " + std::to_string(status);
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
