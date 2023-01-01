#pragma once
#include <functional>
#include <map>
#include <wfmoReturnVal.h>

#define INFINITE -1
#define MAX_EPOLL_EVENTS 16
#define timeoutEventFd -1

typedef std::map<int, int> waitableMap_t;
typedef std::function<bool(int)> wfmoHandlerFunc_t;

class wfmo {
  public:
    wfmo() = delete;
    virtual ~wfmo();
    wfmo(waitableMap_t);
    void setup();
    void teardown();
    wfmoReturnVal_t waitforobject(int, bool bAll = false);
    int findeventid(int);
    void clearlastevent(int);
    void clearAlllastevent();

  private:
    bool handlepollevent(wfmoReturnVal_t &, int, bool);
    bool handleWait(wfmoReturnVal_t &, int);
    bool handleWaitForAll(wfmoReturnVal_t &, int);
    void removeItemFromWaitForAllMap(int);
    int m_epfd;
    struct epoll_event *m_pout_events;
    waitableMap_t m_mapIw;
    waitableMap_t m_mapIwWaitForAll;
};
