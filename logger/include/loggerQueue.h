#pragma once
#include <condition_variable>
#include <deque>
#include <loggerRecord.h>
#include <mutex>

class loggerQueue {
  public:
    loggerQueue() = default;
    virtual ~loggerQueue() = default;
    loggerRecord pop();
    void push(loggerRecord);
    bool isEmpty();
    size_t size();
    void clear();

    mutable std::mutex m_qmutex;
    std::condition_variable m_qcv;
  private:
    std::deque<loggerRecord> m_q;
};
