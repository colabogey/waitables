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

#include <unistd.h>
#include <IRunnableApp.h>
#include <vpgnixApp.h>
#include <vpgnixlogger.h>

class vpgnixDaemon {
  public:
    vpgnixDaemon();
    virtual ~vpgnixDaemon() = default;
    void init();
    void uninit();
    void run();
    void setupSignals();
    void handleSIGHUP();
    void handleSIGTERM();
  private:
    bool m_keepRunning;
    std::shared_ptr<ILogger> m_logger;
    std::shared_ptr<IRunnableApp> m_app;
    int m_sfd;
    pid_t m_pid;
};
