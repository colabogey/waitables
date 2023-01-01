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
