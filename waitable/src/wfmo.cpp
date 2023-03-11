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


#include <eventIds.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <vpgnixexception.h>
#include <wfmo.h>
#include <wfmoReturnVal.h>

wfmo::wfmo(waitableMap_t mapIw) {
    m_mapIw = mapIw;
    m_pout_events = nullptr;
}

void wfmo::setup() {
    struct epoll_event event;
    struct epoll_event *events;
    m_epfd = epoll_create1(0);
    if (m_epfd == -1) {
        std::string err = strerror(errno);
        std::string ex = "wfmo setup: (epoll_create1) " + err;
        throw vpgnixexception(ex.c_str());
    }
    m_pout_events = (epoll_event *)calloc(MAX_EPOLL_EVENTS, sizeof event);
    int j = 0;
    for (const auto &[key, value] : m_mapIw) {
        if(key != -1) { // timeout (-1) is in the map but no epoll stuff
            event.data.fd = key;
            //event.events = EPOLLIN | EPOLLET;
            event.events = EPOLLIN;
            int s = epoll_ctl(m_epfd, EPOLL_CTL_ADD, key, &event);
            if (s == -1) {
                std::string err = strerror(errno);
                std::string ex = "wfmo setup: (epoll_ctl) " + err;
                throw vpgnixexception(ex.c_str());
            }
        }
        j++;
    }
}

void wfmo::teardown() {
    // epoll 'descriptors' are different than the associated file descriptor
    // e.g. you can clse a file descriptor and still get an event (is that
    // broken?) the delete here clras out epoll closing the file descriptors
    // themselves is dealt with by the various user modules
    //
    int j = 0;
    for (const auto &[key, value] : m_mapIw) {
        if(key != -1) { // timeout (-1) is in the map but no epoll stuff
            int s = epoll_ctl(m_epfd, EPOLL_CTL_DEL, key, nullptr);
            if (s == -1) {
                std::string err = strerror(errno);
                std::string ex = "wfmo teardown: (epoll_ctl) " + err;
                throw vpgnixexception(ex.c_str());
            }
        }
        j++;
    }
    if (m_pout_events != nullptr) {
        delete m_pout_events;
        m_pout_events = nullptr;
    }
    m_mapIw.clear();
}

wfmoReturnVal_t wfmo::waitforobject(int timeout, bool bWaitForAll) {
    bool keeplooping = true;
    if(bWaitForAll) {
        m_mapIwWaitForAll = m_mapIw;
    }
    wfmoReturnVal_t rv = {timeoutEventFd, commonEventIds::timeoutEventId};
    while (keeplooping) {
        int nfds = epoll_wait(m_epfd, m_pout_events, MAX_EPOLL_EVENTS, timeout);
        if (nfds == -1) {
            std::string err = strerror(errno);
            std::string ex = "wfmo waitforobject: (epoll_wait) " + err;
            throw vpgnixexception(ex.c_str());
        } else if (nfds == 0) {
            break; // timeout
        } else {
            keeplooping = handlepollevent(rv, nfds, bWaitForAll);
        }
    }
    return rv;
}

bool wfmo::handlepollevent(wfmoReturnVal_t &rv, int nfds, bool bWaitForAll) {
    bool bRet = true;
    if(bWaitForAll) {
        bRet = handleWaitForAll(rv, nfds);
    } else {
        bRet = handleWait(rv, nfds);
    }
    return bRet;
}

bool wfmo::handleWait(wfmoReturnVal_t &rv, int nfds) {
    bool bret = true;
    for (int j = 0; j < nfds; j++) {
        if (m_pout_events[j].events & EPOLLIN) {
            int fd = m_pout_events[j].data.fd;
            int id = findeventid(fd);
            if (id != 0) {
                rv.m_fd = fd;
                rv.m_id = id;
                bret = false;
                break;
            }
        }
    }
    return bret;
}

int wfmo::findeventid(int fd) {
    int ret = 0;
    auto result = m_mapIw.find(fd);
    if (result != m_mapIw.end()) {
        ret = result->second;
    }
    return ret;
}

bool wfmo::handleWaitForAll(wfmoReturnVal_t &rv, int nfds) {
    bool bret = true;
    for (int j = 0; j < nfds; j++) {
        if (m_pout_events[j].events & EPOLLIN) {
            int fd = m_pout_events[j].data.fd;
            removeItemFromWaitForAllMap(fd);
            if(m_mapIwWaitForAll.size() == 0) {
                rv.m_fd = -1;
                rv.m_id = commonEventIds::waitForAllEventId;
                bret = false;
                break;
            }
        }
    }
    return bret;
}

// waiting for all will simply track if all events have fired
// clear the event so there won't be needless refires
// whether it in the map or not
//
void wfmo::removeItemFromWaitForAllMap(int fd) {
    auto result = m_mapIwWaitForAll.find(fd);
    if (result != m_mapIwWaitForAll.end()) {
        m_mapIwWaitForAll.erase(result);
    }
    clearlastevent(fd);
}

void wfmo::clearAlllastevent() {
    for (const auto &[key, value] : m_mapIw) {
        if(key != -1) { // timeout (-1) is in the map but no epoll stuff
            continue;
        }
        clearlastevent(key);
    }
}

void wfmo::clearlastevent(int fd) {
    uint64_t u = 0;
    ssize_t s = read(fd, &u, sizeof(uint64_t));
}

wfmo::~wfmo() {
    m_mapIw.clear();
}

