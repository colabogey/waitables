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
#include <string>
#include <IRunnable.h>
#include <corethread.h>
#include <waitable_thread.h>
#include <wfmo.h>

class vpgnix : public IRunnable {
  public:
    vpgnix(std::shared_ptr<waitable_event>, std::shared_ptr<corethread>,
           std::shared_ptr<waitable_event>, std::shared_ptr<ILogger>);
    virtual ~vpgnix() = default;
    virtual void start() override;
    virtual void stop() override;
    virtual void run(int, std::string &) override;

  private:
    int m_vpgnix_run_waitablethreadfd;
    int m_myStopEventFd;
    void handleMessage(int);
    std::shared_ptr<corethread> m_ct;
    std::shared_ptr<waitable_event> m_ctse;
    std::shared_ptr<waitable_event> m_vpse;
    std::shared_ptr<ILogger> m_logger;
    std::unique_ptr<waitable_thread> m_pwt;
    std::shared_ptr<IWaitableMessageSink> m_message_sink;
};
