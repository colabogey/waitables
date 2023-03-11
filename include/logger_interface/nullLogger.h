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

#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include "cstring.h"
#include <logger_interface.h>
#include <loggerConfig.h>

class nullLogger : public ILogger {
  public:
    nullLogger();
    virtual ~nullLogger();
    virtual void Log(uint32_t nLevel, std::string sData, ...) override;
    virtual void Log(uint32_t nLevel, cstring&) override;
    virtual void Initialize(std::shared_ptr<loggerConfig>) override;
    virtual void Uninitialize() override;

  private:
    nullLogger(nullLogger const &);
    void operator=(nullLogger const &);
};
