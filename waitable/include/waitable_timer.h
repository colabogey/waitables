#pragma once
#include <stdint.h>
#include <sys/timerfd.h>
#include <IWaitable.h>
#include <timerTypes.h>

class waitable_timer :public IWaitable {
  public:
    waitable_timer(int, timerType, std::shared_ptr<ILogger>);
    virtual ~waitable_timer();
    void create();
    void set(uint32_t);
    void reset();
    void stop();
    virtual int getFd() override;
    virtual int getId() override;
  private:
    int m_fd;
    int m_id;
    std::shared_ptr<ILogger> m_logger;
    struct itimerspec m_itToRun;
    struct itimerspec m_itToStop;
    timerType m_timerType;
};
