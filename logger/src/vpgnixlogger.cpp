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

#include <vpgnixlogger.h>

vpgnixlogger::vpgnixlogger() {
    sLoglevel[LOGLEVEL_TRACE] = "[TRACE]";
    sLoglevel[LOGLEVEL_DEBUG] = "[DEBUG]";
    sLoglevel[LOGLEVEL_INFO] = "[INFO ]";
    sLoglevel[LOGLEVEL_WARNING] = "[WARN ]";
    sLoglevel[LOGLEVEL_ERROR] = "[ERROR]";
    sLoglevel[LOGLEVEL_FATAL] = "[FATAL]";
}

vpgnixlogger::~vpgnixlogger() {
    m_lq.clear();
}

void vpgnixlogger::Uninitialize() {
    while(m_lq.size() > 0)
    {
        m_lq.m_qcv.notify_all();
        sleep(1);
    }
    m_brunning = false;
    fflush(m_file);
    fclose(m_file);
}

void vpgnixlogger::Initialize(std::shared_ptr<loggerConfig> lc) {
    m_logFileName = lc->getFn();
    m_lvl = lc->getLvl();
    m_file = fopen(m_logFileName.c_str(), "a+");
    m_brunning = true;
    m_t = std::move(std::thread([=, this]() { vpgnixlogger::write(); }));
    m_t.detach();
}

void vpgnixlogger::Log(uint32_t nLevel, std::string sData, ...) {
    if(nLevel < m_lvl) {
        return;
    }
    loggerRecord lr(nLevel);
    lr.m_time = std::time(nullptr);
    lr.m_tid = gettid();
    va_list args1;
    va_start(args1, sData);
    va_list args2;
    va_copy(args2, args1);
    lr.m_text.resize(1 + std::vsnprintf(nullptr, 0, sData.c_str(), args1));
    va_end(args1);
    vsnprintf(lr.m_text.data(), lr.m_text.size(), sData.c_str(), args2);
    va_end(args2);
    m_lq.push(lr);
}

void vpgnixlogger::Log(uint32_t nLevel, cstring& s) {
    if(nLevel < m_lvl) {
        return;
    }
    loggerRecord lr(nLevel);
    lr.m_time = std::time(nullptr);
    lr.m_tid = gettid();
    //lr.m_text.resize(1 + std::snprintf(nullptr, 0, s.data()));
    //snprintf(lr.m_text.data(), lr.m_text.size(), s.data());
    m_lq.push(lr);
}

void vpgnixlogger::write() {
    while (m_brunning) {
        while(!m_lq.isEmpty()) {
            loggerRecord lr = m_lq.pop();
            // TODO: get a better idea in the size to allocate by not allocating enough
            //  strftime will return 0. So make the size be bigger until its happy
            //  like maybe start at 25 and double every time through a loop
            //  very low priority - but nerd-olicious
            //
            std::vector<char> timeVec(100);
            long ms = lr.m_tval.tv_usec/1000; // calculate milliseconds
            std::strftime(timeVec.data(), timeVec.size(), "%D %T", localtime(&lr.m_time));
            // find out how much to allocate
            int len = snprintf(nullptr, 0, "%s.%03ld %s [%08X]: %s\n", 
                    timeVec.data(), 
                    ms,
                    sLoglevel[lr.m_level].c_str(), 
                    lr.m_tid,
                    lr.m_text.data());

            std::vector<char> dataVec(len+1);
            snprintf(dataVec.data(), dataVec.size(), "%s.%03ld %s [%08X]: %s\n", 
                    timeVec.data(), 
                    ms,
                    sLoglevel[lr.m_level].c_str(), 
                    lr.m_tid,
                    lr.m_text.data());

            fprintf(stderr, "%s", dataVec.data());
            fwrite(dataVec.data(), 1, strlen(dataVec.data()), m_file);
            fflush(m_file);
        }
    }
}
