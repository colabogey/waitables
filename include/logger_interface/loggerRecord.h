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
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <vector>

class loggerRecord {
  public:
    loggerRecord(uint32_t level)
        : m_time(std::time(nullptr))
        , m_level(level)
        , m_tid(gettid())
        { 
            gettimeofday(&m_tval, nullptr);
        };

    loggerRecord() = default;
    virtual ~loggerRecord() = default;

    std::vector<char> m_text;
    std::time_t m_time;
    struct timeval m_tval;
    uint32_t m_level;
    pid_t m_tid;
};
