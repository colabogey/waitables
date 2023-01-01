#pragma once

#include <string>
#include <memory>
#include "cstring.h"
#include <loggerConfig.h>

class ILogger {
  public:
    virtual ~ILogger() = default;
    virtual void Initialize(const std::shared_ptr<loggerConfig>) = 0;
    virtual void Uninitialize() = 0;
    virtual void Log(uint32_t nLevel, std::string sData, ...) = 0;
    virtual void Log(uint32_t nLevel, cstring&) = 0;
};
