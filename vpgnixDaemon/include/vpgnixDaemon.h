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
