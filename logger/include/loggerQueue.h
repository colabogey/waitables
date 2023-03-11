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
#include <condition_variable>
#include <deque>
#include <loggerRecord.h>
#include <mutex>

class loggerQueue {
  public:
    loggerQueue() = default;
    virtual ~loggerQueue() = default;
    loggerRecord pop();
    void push(loggerRecord);
    bool isEmpty();
    size_t size();
    void clear();

    mutable std::mutex m_qmutex;
    std::condition_variable m_qcv;
  private:
    std::deque<loggerRecord> m_q;
};
