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

#pragma once
#include <functional>
#include <map>
#include <wfmoReturnVal.h>

#define INFINITE -1
#define MAX_EPOLL_EVENTS 16
#define timeoutEventFd -1

typedef std::map<int, int> waitableMap_t;
typedef std::function<bool(int)> wfmoHandlerFunc_t;

class wfmo {
  public:
    wfmo() = delete;
    virtual ~wfmo();
    wfmo(waitableMap_t);
    void setup();
    void teardown();
    wfmoReturnVal_t waitforobject(int, bool bAll = false);
    int findeventid(int);
    void clearlastevent(int);
    void clearAlllastevent();

  private:
    bool handlepollevent(wfmoReturnVal_t &, int, bool);
    bool handleWait(wfmoReturnVal_t &, int);
    bool handleWaitForAll(wfmoReturnVal_t &, int);
    void removeItemFromWaitForAllMap(int);
    int m_epfd;
    struct epoll_event *m_pout_events;
    waitableMap_t m_mapIw;
    waitableMap_t m_mapIwWaitForAll;
};
