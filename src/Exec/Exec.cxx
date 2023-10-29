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

#include "Exec.hxx"
#include <sstream>
#include <array>
#include <stdio.h>

Exec::Exec(std::string binary, std::vector<std::string> args)
        : binary_(binary), args_(args) {

}

std::tuple<int, std::string> Exec::output() {
    auto cmd = get_command();
    std::array<char, 128> buffer;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int status = WEXITSTATUS(pclose(pipe));
    return {status, result};
}

std::string Exec::get_command() {
    std::stringstream ss;
    ss << binary_;
    for (auto const &arg: args_) {
        ss << " " << arg;
    }
    ss << " 2>&1";

    return ss.str();
}
