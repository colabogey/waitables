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
