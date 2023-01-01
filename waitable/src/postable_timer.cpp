#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <vpgnixexception.h>
#include <messageValues.h>
#include <timerTypes.h>
#include <waitable_messageSource.h>
#include <postable_timer.h>

postable_timer::postable_timer(int id, 
                               timerType tt,
                               std::string msgDestination, 
                               std::shared_ptr<IWaitableMessageSource> sp_wms,
                               std::shared_ptr<ILogger> sp_logger) {
    m_id = id;
    m_wms = sp_wms;
    m_logger = sp_logger;
    m_timerType = tt;
    m_msgDestination = msgDestination;
    memset(&m_itToRun, 0, sizeof(struct itimerspec));
    memset(&m_itToStop, 0, sizeof(struct itimerspec));
    m_sev.sigev_notify = SIGEV_THREAD;
    m_sev.sigev_value.sival_ptr = (void*) this;
    m_sev.sigev_notify_function = &postable_timer::handler;
    m_sev.sigev_notify_attributes = 0;
}

int postable_timer::create() {
    int rc = -1;
    m_logger->Log(LOGLEVEL_INFO, "postable_timer create");
    rc = timer_create(CLOCK_REALTIME, &m_sev, &m_timerid);
    if(rc == -1) {
        std::string err = strerror(errno);
        std::string ex = "postable_timer:" + err;
        m_logger->Log(LOGLEVEL_ERROR, "postable_timer: timer_create failed (%s)", err);
        throw vpgnixexception(ex.c_str());
    }
    return rc;
}

void postable_timer::handler(sigval_t val) {
    postable_timer* pt = (postable_timer *)val.sival_ptr;
    pt->expired();
}

void postable_timer::expired() {
    m_logger->Log(LOGLEVEL_INFO, "postable_timer expired");
    m_wms->postThreadMessage(getMsgDestination(), WM_TIMER, getId(), 0);
}

void postable_timer::set(uint32_t ms) {
    m_itToRun.it_value.tv_sec = ms / 1000;
    m_itToRun.it_value.tv_nsec = (ms % 1000) * 1000000;
    if(m_timerType == timerType::PERIODIC) {
        m_itToRun.it_interval.tv_sec = ms / 1000;
        m_itToRun.it_interval.tv_nsec = (ms % 1000) * 1000000;
    }
    int rc = timer_settime(m_timerid, 0, &m_itToRun, nullptr);
    m_logger->Log(LOGLEVEL_INFO, 
                "postable_timer settime said (%d) value[(%ld) (%ld)] interval[(%ld) (%ld)]", 
                 rc, 
                 m_itToRun.it_value.tv_sec, m_itToRun.it_value.tv_nsec,
                 m_itToRun.it_interval.tv_sec, m_itToRun.it_interval.tv_nsec);
}

void postable_timer::reset() {
    int rc = timer_settime(m_timerid, 0, &m_itToRun, nullptr);
}
void postable_timer::stop() {
    int rc = timer_settime(m_timerid, 0, &m_itToStop, nullptr);
}

postable_timer::~postable_timer() {
    stop();
}

