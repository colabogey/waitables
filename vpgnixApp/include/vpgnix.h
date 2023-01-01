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
