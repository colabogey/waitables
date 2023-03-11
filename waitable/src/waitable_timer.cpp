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

#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>
#include <vpgnixlogger.h>
#include <vpgnixexception.h>
#include <waitable_timer.h>

waitable_timer::waitable_timer(int id, timerType tt,  std::shared_ptr<ILogger> sp_logger) {
    m_fd = -1;
    m_id = id;
    m_timerType = tt;
    m_logger = sp_logger;
    memset(&m_itToRun, 0, sizeof(struct itimerspec));
    memset(&m_itToStop, 0, sizeof(struct itimerspec));
}
void waitable_timer::create() {
    m_fd = timerfd_create(CLOCK_REALTIME, 0);
    m_logger->Log(LOGLEVEL_INFO, "waitable_timer timerfd_create (%d)", m_fd);
    if(m_fd == -1) {
        m_logger->Log(LOGLEVEL_ERROR,
            "waitable_timer timerfd_create failed (%s)", strerror(errno));
        std::string err = strerror(errno);
        std::string ex = "waitable_timer:" + err;
        throw vpgnixexception(ex.c_str());
    }
}

void waitable_timer::set(uint32_t ms) {
    m_itToRun.it_value.tv_sec = ms / 1000;
    m_itToRun.it_value.tv_nsec = (ms % 1000) * 1000000;
    if(m_timerType == timerType::PERIODIC) {
        m_itToRun.it_interval.tv_sec = ms / 1000;
        m_itToRun.it_interval.tv_nsec = (ms % 1000) * 1000000;
    }
    m_logger->Log(LOGLEVEL_INFO,
        "waitable_timer timerfd_settime (%d) value[(%ld) (%ld)] interval[(%ld) (%ld)]",
        m_fd,
        m_itToRun.it_value.tv_sec, m_itToRun.it_value.tv_nsec,
        m_itToRun.it_interval.tv_sec, m_itToRun.it_interval.tv_nsec);
    int rc = timerfd_settime(m_fd, 0, &m_itToRun, nullptr);
    if(rc == -1) {
        m_logger->Log(LOGLEVEL_ERROR,
            "waitable_timer timerfd_settime failed (%d) (%s)", errno, strerror(errno));
    }
}

void waitable_timer::reset() {
    int rc = timerfd_settime(m_fd, 0, &m_itToRun, nullptr);
}
void waitable_timer::stop() {
    int rc = timerfd_settime(m_fd, 0, &m_itToStop, nullptr);
}

waitable_timer::~waitable_timer() {
    if (m_fd != -1) {
        stop();
        close(m_fd);
        m_fd = -1;
    }
}

int waitable_timer::getFd() { 
    return m_fd; 
};

int waitable_timer::getId() { 
    return m_id; 
};

