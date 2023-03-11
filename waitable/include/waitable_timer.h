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
#include <sys/timerfd.h>
#include <IWaitable.h>
#include <timerTypes.h>

class waitable_timer :public IWaitable {
  public:
    waitable_timer(int, timerType, std::shared_ptr<ILogger>);
    virtual ~waitable_timer();
    void create();
    void set(uint32_t);
    void reset();
    void stop();
    virtual int getFd() override;
    virtual int getId() override;
  private:
    int m_fd;
    int m_id;
    std::shared_ptr<ILogger> m_logger;
    struct itimerspec m_itToRun;
    struct itimerspec m_itToStop;
    timerType m_timerType;
};
