
#include <aspthread.h>
#include <eventIds.h>
#include <messageValues.h>
#include <postable_timer.h>
#include <sys/eventfd.h>
#include <threadNames.h>
#include <timerEventIds.h>
#include <timerTypes.h>
#include <unistd.h>
#include <vector>
#include <vpgnixlogger.h>
#include <waitable_event.h>
#include <waitable_message.h>
#include <waitable_messageSource.h>
#include <waitable_thread.h>
#include <waitable_timer.h>
#include <wfmoReturnVal.h>

aspthread::aspthread(std::shared_ptr<waitable_event> sp_atse,
                     std::shared_ptr<ILogger> sp_logger) {
    m_atse = sp_atse;
    m_logger = sp_logger;
    m_goofyThrottle = 0;
}

void aspthread::start() {
    m_logger->Log(LOGLEVEL_TRACE, "at start: enter");
    std::string myThreadName(threadNames::aspthreadRun);
    waitableThreadFunc_t f = std::bind(
        &aspthread::run, this, std::placeholders::_1, std::placeholders::_2);
    m_pwt = std::make_unique<waitable_thread>(
        myThreadName, aspthreadEventIds::runWaitableThreadId, f);
    m_pwt->func();
    m_logger->Log(LOGLEVEL_TRACE, "at start: started waitable_thread (%d) (%s)",
                  m_pwt->getFd(), myThreadName.c_str());
    m_logger->Log(LOGLEVEL_TRACE, "at start: exit");
}

void aspthread::run(int fd, std::string &myThreadName) {
    m_logger->Log(LOGLEVEL_TRACE, "at run: enter (%d) (%s)", fd,
                  myThreadName.c_str());
    //
    std::vector<std::string> destinationThreads;
    destinationThreads.push_back(std::string(threadNames::corethreadRun));
    destinationThreads.push_back(std::string(threadNames::vpgateRun));
    //
    for (const auto &s : destinationThreads) {
        m_logger->Log(LOGLEVEL_INFO, "at run: destination threads (%s)",
                      s.c_str());
    }
    //
    m_message_source =
        std::make_shared<waitableMessageSource>(myThreadName, m_logger);
    m_message_source->initialize(destinationThreads);
    //
    m_wt = std::make_shared<waitable_timer>(
        aspthreadWaitableTimerEventIds::aspthreadWaitableTimer,
        timerType::PERIODIC, m_logger);
    m_wt->create();
    m_wt->set(3000);
    //
    //
    waitableMap_t myWaitableMap; // stop event and waitable timer
    myWaitableMap.insert(std::pair<int, int>(m_atse->getFd(), m_atse->getId()));
    myWaitableMap.insert(std::pair<int, int>(m_wt->getFd(), m_wt->getId()));
    //
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    //
    m_pt = std::make_shared<postable_timer>(
        aspthreadPostableTimerEventIds::ASPThreadHealth, timerType::ONESHOT,
        std::string(threadNames::corethreadRun), m_message_source, m_logger);

    m_pt->create();
    m_pt->set(1000);
    //
    bool brunning = true;
    while (brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(INFINITE);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (at run): m_fd (%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        //
        if (rv.m_id == m_atse->getId()) {
            m_logger->Log(LOGLEVEL_DEBUG, "wfmo (at run): stop event signaled");
            my_wfmo->clearlastevent(rv.m_fd);
            brunning = false;
        } else if (rv.m_id == m_wt->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            handleWaitableTimer();
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "at run: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    }
    m_logger->Log(LOGLEVEL_TRACE, "at run: exit  (%d) (%s)", fd,
                  myThreadName.c_str());
    myWaitableMap.clear();
    my_wfmo->teardown();
    m_pwt->signalthreadexit();
}

void aspthread::handleWaitableTimer() {
    m_logger->Log(LOGLEVEL_INFO, "handleWaitableTimer (%d)", m_goofyThrottle);
    if (m_goofyThrottle <= 12) {
        m_goofyThrottle++;
        postSomeMessages();
        m_pt->set(1000);
    } else {
        m_logger->Log(
            LOGLEVEL_INFO,
            "handleWaitableTimer stopping timers and message generation");
        m_pt->stop();
        m_wt->stop();
    }
}

void aspthread::stop() {
    waitableMap_t myWaitableMap; // thread exit and timeout
    myWaitableMap.insert(std::pair<int, int>(m_pwt->getFd(), m_pwt->getId()));
    myWaitableMap.insert(
        std::pair<int, int>(timeoutEventFd, commonEventIds::timeoutEventId));

    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();

    m_logger->Log(
        LOGLEVEL_DEBUG,
        "at stop: signal run() to stop and wait for thread exit event");
    m_atse->reset_event();

    bool brunning = true;
    while (brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(5000);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (at stop): m_fd (%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        if (rv.m_id == m_pwt->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            brunning = false;
        } else if (rv.m_id == commonEventIds::timeoutEventId) {
            brunning = false;
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "at stop: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    }
    m_logger->Log(LOGLEVEL_TRACE, "at stop: exit");
    myWaitableMap.clear();
    my_wfmo->teardown();
}

void aspthread::postSomeMessages() {
    for (int i = 1; i <= 3; i++) {
        m_message_source->postThreadMessage(
            std::string(threadNames::corethreadRun), WM_CORE_ASP_MSG1, i,
            i + 100);
        m_message_source->postThreadMessage(std::string(threadNames::vpgateRun),
                                            WM_CORE_ASP_MSG2, i, i + 100);
    }
}
