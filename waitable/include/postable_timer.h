#pragma once
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <timerTypes.h>
#include <waitable_messageSource.h>

class postable_timer {
  public:
    postable_timer() = delete;
    postable_timer(int, 
                   timerType,
                   std::string, 
                   std::shared_ptr<IWaitableMessageSource>,
                   std::shared_ptr<ILogger> sp_logger);
    virtual ~postable_timer();
    static void handler(sigval_t val);
    int create();
    void set(uint32_t);
    void reset();
    void stop();
    void expired();
    int getId() { return m_id; };
    std::string getMsgDestination() { return m_msgDestination; };
  private:
    int m_id;
    std::string m_msgDestination;
    std::shared_ptr<IWaitableMessageSource> m_wms;
    std::shared_ptr<ILogger> m_logger;
    timer_t m_timerid;
    struct sigevent m_sev;
    struct itimerspec m_itToRun;
    struct itimerspec m_itToStop;
    timerType m_timerType;
};
