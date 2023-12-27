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

#ifndef INITIAL_SETUP_WORKER_HXX
#define INITIAL_SETUP_WORKER_HXX

#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <poll.h>
class Window;

class Worker {
private:
    mutable std::mutex mutex_;
    FILE *pipe_;
    int fd_;
    bool completed{false};
    pollfd fds[1];
    bool has_stopped_;
    double progress_;
    Glib::ustring message_;
public:
    Worker();

    void start(Window *caller);

    void get_data(double *fraction_done, Glib::ustring *message);

    bool has_stopped() const;
};


#endif //INITIAL_SETUP_WORKER_HXX
