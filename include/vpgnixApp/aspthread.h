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
#include <postable_timer.h>
#include <string>
#include <IRunnable.h>
#include <vpgnixlogger.h>
#include <waitable_event.h>
#include <waitable_messageSource.h>
#include <waitable_thread.h>
#include <waitable_timer.h>
#include <wfmo.h>

class aspthread : public IRunnable {

  public:
    aspthread(std::shared_ptr<waitable_event>, std::shared_ptr<ILogger>);
    virtual ~aspthread() = default;
    virtual void start() override;
    virtual void stop() override;
    virtual void run(int, std::string &) override;

  private:
    bool startCoreProcessingThreads();
    bool startAspThread();
    void postSomeMessages();
    void handleWaitableTimer();
    std::shared_ptr<postable_timer> m_pt;
    std::shared_ptr<waitable_timer> m_wt;
    std::shared_ptr<ILogger> m_logger;
    std::unique_ptr<waitable_thread> m_pwt;
    std::shared_ptr<waitable_event> m_atse;
    std::shared_ptr<IWaitableMessageSource> m_message_source;
    int m_goofyThrottle;
};
