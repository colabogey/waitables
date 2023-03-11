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
#include <cstring>
#include "cstring.h"
#include <cstdio>
#include <ctime>
#include <loggerQueue.h>
#include <loggerRecord.h>
#include <logger_interface.h>
#include <mutex>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <loggingLevel.h>
#include <loggerConfig.h>
#include <vpgnixlogger.h>

class vpgnixlogger : public ILogger {
  public:
    vpgnixlogger();
    virtual ~vpgnixlogger();
    virtual void Log(uint32_t nLevel, std::string sData, ...) override;
    virtual void Log(uint32_t nLevel, cstring&) override;
    virtual void Initialize(std::shared_ptr<loggerConfig>) override;
    virtual void Uninitialize() override;

  private:
    vpgnixlogger(vpgnixlogger const &);
    void operator=(vpgnixlogger const &);
    void write();
    //
    uint32_t    m_lvl;
    //
    std::string m_logFileName;
    FILE* m_file;
    std::string sLoglevel[6];
    std::mutex m_logmutex;
    loggerQueue m_lq;
    std::thread m_t;
    bool m_brunning;
};
