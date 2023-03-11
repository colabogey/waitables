/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


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
