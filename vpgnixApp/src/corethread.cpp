
#include <corethread.h>
#include <eventIds.h>
#include <messageValues.h>
#include <postable_timer.h>
#include <sys/eventfd.h>
#include <threadNames.h>
#include <unistd.h>
#include <waitable_event.h>
#include <waitable_messageSink.h>
#include <waitable_thread.h>
#include <wfmoReturnVal.h>

corethread::corethread(std::shared_ptr<waitable_event> sp_ctse,
                       std::shared_ptr<aspthread> sp_at,
                       std::shared_ptr<waitable_event> sp_atse,
                       std::shared_ptr<ILogger> sp_logger) {
    m_ctse = sp_ctse;
    m_at = sp_at;
    m_atse = sp_atse;
    m_logger = sp_logger;
}

void corethread::start() {
    m_logger->Log(LOGLEVEL_TRACE, "ct start: enter");
    std::string myThreadName(threadNames::corethreadRun);
    waitableThreadFunc_t f = std::bind(
        &corethread::run, this, std::placeholders::_1, std::placeholders::_2);

    m_pwt = std::make_unique<waitable_thread>(
        myThreadName, corethreadEventIds::runWaitableThreadId, f);
    m_pwt->func();
    m_logger->Log(LOGLEVEL_TRACE, "ct start: started waitable_thread (%d) (%s)",
                  m_pwt->getFd(), myThreadName.c_str());
    m_logger->Log(LOGLEVEL_TRACE, "ct start: exit");
}

void corethread::run(int fd, std::string &myThreadName) {
    m_logger->Log(LOGLEVEL_TRACE, "ct run: enter (%d) (%s)", fd,
                  myThreadName.c_str());
    //
    m_message_sink = std::make_shared<waitableMessageSink>(
        myThreadName, corethreadEventIds::messageSinkId, m_logger);
    m_message_sink->initialize();
    //
    bool bRunning = startCoreProcessingThreads();
    //
    waitableMap_t myWaitableMap; // stop event and sink
    myWaitableMap.insert(std::pair<int, int>(m_ctse->getFd(), m_ctse->getId()));
    myWaitableMap.insert(
        std::pair<int, int>(m_message_sink->getFd(), m_message_sink->getId()));
    //
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    //
    waitable_message wm;
    bool brunning = true;
    do {
        while (m_message_sink->peekMessage(&wm, true)) {
            switch (wm.message) {
            case WM_CORE_ASP_MSG1:
                handleCoreAspMessage(wm.wParam, wm.lParam);
                break;

            case WM_TIMER:
                timeout(wm.wParam, wm.lParam);
                break;

            default:
                m_logger->Log(LOGLEVEL_WARNING, "ct peek said:(%d) (%ld) (%ld)",
                              wm.message, wm.wParam, wm.lParam);
                break;
            }
        }

        wfmoReturnVal_t rv = my_wfmo->waitforobject(INFINITE);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (ct run): m_fd(%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        //
        if (rv.m_id == m_ctse->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            m_logger->Log(LOGLEVEL_DEBUG, "ct run: stop event signaled");
            brunning = false;
        } else if (rv.m_id == m_message_sink->getId()) {
            m_logger->Log(LOGLEVEL_DEBUG, "ct run: message on queue event");
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "ct run: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    } while (brunning);

    m_logger->Log(LOGLEVEL_TRACE, "ct run: exit  (%d) (%s)", fd,
                  myThreadName.c_str());
    myWaitableMap.clear();
    my_wfmo->teardown();
    m_pwt->signalthreadexit();
}

void corethread::handleCoreAspMessage(uint64_t wParam, uint64_t lParam) {
    m_logger->Log(LOGLEVEL_INFO, "ct handleCoreAspMessage: (%ld) (%ld)", wParam,
                  lParam);
}

void corethread::timeout(uint64_t wParam, uint64_t lParam) {
    m_logger->Log(LOGLEVEL_INFO, "ct timeout: (%ld) (%ld)", wParam, lParam);
}

void corethread::stop() {
    waitableMap_t myWaitableMap; // thread exit and timeout
    myWaitableMap.insert(std::pair<int, int>(m_pwt->getFd(), m_pwt->getId()));
    myWaitableMap.insert(
        std::pair<int, int>(timeoutEventFd, commonEventIds::timeoutEventId));

    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();

    m_logger->Log(LOGLEVEL_DEBUG, "ct stop: signal aspthread run() to stop");
    m_at->stop();

    m_logger->Log(
        LOGLEVEL_DEBUG,
        "ct stop: signal ct run() to stop and wait for thread exit event");
    m_ctse->reset_event();

    bool brunning = true;
    while (brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(5000);
        m_logger->Log(LOGLEVEL_INFO, "wfmo (ct stop): m_fd(%d) m_id(%d)",
                      rv.m_fd, rv.m_id);
        if (rv.m_id == m_pwt->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            brunning = false;
        } else if (rv.m_id == commonEventIds::timeoutEventId) {
            brunning = false;
        } else {
            m_logger->Log(LOGLEVEL_ERROR, "ct stop: no eventId match (%d) (%d)",
                          rv.m_fd, rv.m_id);
            brunning = false;
        }
    }
    m_logger->Log(LOGLEVEL_TRACE, "ct stop: exit");
    myWaitableMap.clear();
    my_wfmo->teardown();
}

bool corethread::startCoreProcessingThreads() { return startAspThread(); }

bool corethread::startAspThread() {
    m_at->start();
    return true;
}
