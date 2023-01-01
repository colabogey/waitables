
#include <eventIds.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/eventfd.h>
#include <IRunnableApp.h>
#include <loggerConfig.h>
#include <vpgnixApp.h>
#include <vpgnixlogger.h>
#include <wfmo.h>

vpgnixApp::vpgnixApp(std::shared_ptr<ILogger> sp_logger) {
    // a neophyte composition root
    //
    // TODO: use some dependency injection container - hypodermic ?
    //
    m_logger = sp_logger;

    m_vpse = std::make_shared<waitable_event>();
    m_vpse->create_event(vpgnixEventIds::stopEventId);

    m_ctse = std::make_shared<waitable_event>();
    m_ctse->create_event(corethreadEventIds::stopEventId);

    m_atse = std::make_shared<waitable_event>();
    m_atse->create_event(aspthreadEventIds::stopEventId);

    m_at = std::make_shared<aspthread>(m_atse, // aspthread stop event
                                       m_logger);

    m_ct = std::make_shared<corethread>(m_ctse, // corethread stop event
                                        m_at,   // aspthread object
                                        m_atse, // aspthread stop event
                                        m_logger);

    m_vp = std::make_shared<vpgnix>(m_vpse, // vpgnix stop event
                                    m_ct,   // corethread onject
                                    m_ctse, // corethread stop event
                                    m_logger);
}

void vpgnixApp::start() {
    m_logger->Log(LOGLEVEL_INFO, "vpa: start: calling vp start");
    m_vp->start();
}

void vpgnixApp::stop() {
    m_logger->Log(LOGLEVEL_INFO, "vpa atop: calling vp stop");
    m_vp->stop();
}
