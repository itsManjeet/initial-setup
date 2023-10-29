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

#ifndef INITIAL_SETUP_EXEC_HXX
#define INITIAL_SETUP_EXEC_HXX

#include <string>
#include <vector>
#include <tuple>

class Exec {
private:
    std::string binary_;
    std::vector<std::string> args_;

    std::string get_command();

public:
    Exec(std::string binary, std::vector<std::string> args);

    Exec &arg(std::string const &arg) {
        args_.push_back(arg);
        return *this;
    }

    std::tuple<int, std::string> output();
};


#endif //INITIAL_SETUP_EXEC_HXX
