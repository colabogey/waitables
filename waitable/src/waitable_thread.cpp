/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <sys/eventfd.h>
#include <thread>
#include <string>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vpgnixexception.h>
#include <waitable_thread.h>

waitable_thread::waitable_thread(std::string& myThreadName, 
                                 int id,
                                 std::function<void(int, std::string&)> f) {
    m_id = id;
    m_f = f;
    m_fd = eventfd(0, 0);
    if(m_fd == -1) {
        std::string err = strerror(errno);
        std::string ex = "waitable_thread:" + err;
        throw vpgnixexception(ex.c_str());
    }
    m_myThreadName = myThreadName;
}

void waitable_thread::func() {
    m_t = std::move(std::thread([=, this]() {
        m_f(m_fd, m_myThreadName);
        return 1;
    }));
    m_t.detach();
}

void waitable_thread::signalthreadexit() {
    // fprintf(stderr, "waitable thread signalthreadexit (%d)\n", m_fd);
    uint64_t u = 1;
    ssize_t s = write(m_fd, &u, sizeof(uint64_t));
}

std::string waitable_thread::getName() {
    return m_myThreadName;
}

int waitable_thread::getFd() {
    return m_fd;
}

int waitable_thread::getId() {
    return m_id;
}

waitable_thread::~waitable_thread() {
    // fprintf(stderr, "waitable thread destructor\n");
    if(m_fd != -1) {
        close(m_fd);
        m_fd = -1;
    }
}
