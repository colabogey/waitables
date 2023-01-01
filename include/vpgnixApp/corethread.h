#pragma once

#include <aspthread.h>
#include <functional>
#include <map>
#include <string>
#include <IRunnable.h>
#include <vpgnixlogger.h>
#include <waitable_event.h>
#include <waitable_messageSink.h>
#include <waitable_thread.h>
#include <wfmo.h>

class corethread : public IRunnable {

  public:
    corethread(std::shared_ptr<waitable_event>, std::shared_ptr<aspthread>,
               std::shared_ptr<waitable_event>, std::shared_ptr<ILogger>);
    virtual ~corethread() = default;
    virtual void start() override;
    virtual void stop() override;
    virtual void run(int, std::string &) override;

  private:
    bool startCoreProcessingThreads();
    bool startAspThread();
    void handleCoreAspMessage(uint64_t, uint64_t);
    void timeout(uint64_t, uint64_t);
    std::shared_ptr<waitable_event> m_ctse;
    std::shared_ptr<aspthread> m_at;
    std::shared_ptr<waitable_event> m_atse;
    std::shared_ptr<ILogger> m_logger;
    std::unique_ptr<waitable_thread> m_pwt;
    std::shared_ptr<IWaitableMessageSink> m_message_sink;
};
