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
