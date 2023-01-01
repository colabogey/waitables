#include <eventIds.h>
#include <sys/eventfd.h>
#include <threadNames.h>
#include <unistd.h>
#include <vpgnix.h>
#include <waitable_event.h>
#include <waitable_messageSink.h>
#include <waitable_thread.h>
#include <wfmo.h>
#include <wfmoReturnVal.h>

vpgnix::vpgnix(std::shared_ptr<waitable_event> sp_vpse,
               std::shared_ptr<corethread> sp_ct,
               std::shared_ptr<waitable_event> sp_ctse,
               std::shared_ptr<ILogger> sp_logger) {
    m_vpse = sp_vpse;
    m_ct = sp_ct;
    m_ctse = sp_ctse;
    m_logger = sp_logger;
}

void vpgnix::start() {
    m_logger->Log(LOGLEVEL_TRACE, "vp start: enter");
    std::string myThreadName(threadNames::vpgateRun);
    waitableThreadFunc_t f = std::bind(
        &vpgnix::run, this, std::placeholders::_1, std::placeholders::_2);

    m_pwt = std::make_unique<waitable_thread>(
        myThreadName, vpgnixEventIds::runWaitableThreadId, f);
    m_pwt->func();
    m_logger->Log(LOGLEVEL_TRACE, "vp start: started waitable_thread (%d) (%s)",
                  m_vpgnix_run_waitablethreadfd, myThreadName.c_str());
    m_logger->Log(LOGLEVEL_TRACE, "vp start: exit");
}

void vpgnix::run(int fd, std::string &myThreadName) {
    m_logger->Log(LOGLEVEL_TRACE, "vp run: enter  (%d) (%s)", fd,
                  myThreadName.c_str());

    m_message_sink = std::make_shared<waitableMessageSink>(
        myThreadName, vpgnixEventIds::messageSinkId, m_logger);
    m_message_sink->initialize();
    m_ct->start();
    m_logger->Log(LOGLEVEL_TRACE, "vp run: started corethread thread");
    //
    waitableMap_t myWaitableMap; // stop event and sink
    myWaitableMap.insert(std::pair<int, int>(m_vpse->getFd(), m_vpse->getId()));
    myWaitableMap.insert(
        std::pair<int, int>(m_message_sink->getFd(), m_message_sink->getId()));
    //
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    //
    bool brunning = true;
    while (brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(INFINITE);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (vp run): m_fd(%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        if (rv.m_id == m_vpse->getId()) {
            m_logger->Log(LOGLEVEL_DEBUG, "vp run: stop event signled");
            my_wfmo->clearlastevent(rv.m_fd);
            brunning = false;
        } else if (rv.m_id == m_message_sink->getId()) {
            handleMessage(rv.m_fd);
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "vp run: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    }
    myWaitableMap.clear();
    my_wfmo->teardown();
    m_logger->Log(LOGLEVEL_TRACE, "vp run: exit  (%d) (%s)", fd,
                  myThreadName.c_str());
    m_pwt->signalthreadexit();
}

void vpgnix::handleMessage(int fd) {
    // this gut would do the beloved switch
    waitable_message wm;
    m_message_sink->getMessage(&wm);
    m_logger->Log(LOGLEVEL_INFO, "vp run: handleMessage (%d) (%d) (%d)",
                  wm.message, wm.wParam, wm.lParam);
}

void vpgnix::stop() {
    m_logger->Log(LOGLEVEL_TRACE, "vp stop");
    waitableMap_t myWaitableMap; // thread exit and timeout
    myWaitableMap.insert(std::pair<int, int>(m_pwt->getFd(), m_pwt->getId()));
    myWaitableMap.insert(
        std::pair<int, int>(timeoutEventFd, commonEventIds::timeoutEventId));
    //
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    //
    m_logger->Log(LOGLEVEL_TRACE, "vp stop: calling corethread stop");
    m_ct->stop();

    m_logger->Log(
        LOGLEVEL_DEBUG,
        "vp stop: signal vp run() to stop and wait for thread exit event");
    m_vpse->reset_event();

    bool brunning = true;
    while (brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(5000);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (vp stop): m_fd(%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        if (rv.m_id == m_pwt->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            brunning = false;
        } else if (rv.m_id == commonEventIds::timeoutEventId) {
            brunning = false;
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "vp stop: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    }
    m_logger->Log(LOGLEVEL_TRACE, "vp stop: exit");
    myWaitableMap.clear();
    my_wfmo->teardown();
}
