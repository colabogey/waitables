#pragma once
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <vector>

class loggerRecord {
  public:
    loggerRecord(uint32_t level)
        : m_time(std::time(nullptr))
        , m_level(level)
        , m_tid(gettid())
        { 
            gettimeofday(&m_tval, nullptr);
        };

    loggerRecord() = default;
    virtual ~loggerRecord() = default;

    std::vector<char> m_text;
    std::time_t m_time;
    struct timeval m_tval;
    uint32_t m_level;
    pid_t m_tid;
};
