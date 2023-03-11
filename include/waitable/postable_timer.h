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
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <timerTypes.h>
#include <waitable_messageSource.h>

class postable_timer {
  public:
    postable_timer() = delete;
    postable_timer(int, 
                   timerType,
                   std::string, 
                   std::shared_ptr<IWaitableMessageSource>,
                   std::shared_ptr<ILogger> sp_logger);
    virtual ~postable_timer();
    static void handler(sigval_t val);
    int create();
    void set(uint32_t);
    void reset();
    void stop();
    void expired();
    int getId() { return m_id; };
    std::string getMsgDestination() { return m_msgDestination; };
  private:
    int m_id;
    std::string m_msgDestination;
    std::shared_ptr<IWaitableMessageSource> m_wms;
    std::shared_ptr<ILogger> m_logger;
    timer_t m_timerid;
    struct sigevent m_sev;
    struct itimerspec m_itToRun;
    struct itimerspec m_itToStop;
    timerType m_timerType;
};
