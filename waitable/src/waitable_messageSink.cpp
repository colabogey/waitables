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

#include <string>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <threadNames.h>
#include <waitable_messageSink.h>
#include <vpgnixlogger.h>
#include <vpgnixexception.h>

waitableMessageSink::waitableMessageSink(std::string name,
                                         int id,
                                         std::shared_ptr<ILogger> sp_logger)
{
    m_myName = name;
    m_myQueueName = formQueueName(name);
    m_myId = id;
    m_myFd = -1;
    m_logger = sp_logger;
}

waitableMessageSink::~waitableMessageSink()
{
    close(m_myFd);  // the member not the system call TODO: is this confusing ?
}

bool waitableMessageSink::peekMessage(waitable_message* Pmessage, bool bRemove) 
{
    bool bMessageAvailable = false;
    struct mq_attr q_attr;
    int rv = mq_getattr(m_myFd, &q_attr);
    m_logger->Log(LOGLEVEL_INFO, "peekmessage: mq_curmsgs (%d)", q_attr.mq_curmsgs);
    if(q_attr.mq_curmsgs != 0)
    {
        bMessageAvailable = true;
        if(bRemove)
        {
            ssize_t amount = recv(Pmessage, sizeof(waitable_message));
            m_logger->Log(LOGLEVEL_INFO, "peekmessage: recv (%d)", amount);
        }
    }
    return bMessageAvailable;
}

bool waitableMessageSink::getMessage(waitable_message* Pmessage) 
{
    bool bMessageAvailable = false;
    ssize_t amount = recv(Pmessage, sizeof(waitable_message));
    if (amount != -1) {
        bMessageAvailable = true;
    }
    return bMessageAvailable;
}

void waitableMessageSink::initialize() 
{
    // should set queue attrs as an administrator via /proc
    m_queueAttr.mq_maxmsg = 1024;
    m_queueAttr.mq_msgsize = sizeof(waitable_message);
    m_myFd = open(m_myQueueName);
}

int waitableMessageSink::open(const std::string& queueName) 
{
    mqd_t fd = mq_open(queueName.c_str(), 
                     O_CREAT | O_RDWR, 
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
                     &m_queueAttr);
    if(fd == -1) {
        std::string err = strerror(errno);
        std::string ex = "waitable_event:" + err;
        m_logger->Log(LOGLEVEL_ERROR, "wmsink: mq_open failed (%s)", err);
        throw vpgnixexception(ex.c_str());
    }
    return (int)fd;
}

int waitableMessageSink::close(int fd) 
{
    int rc = -1;
    if(fd != -1) {
        rc = mq_close(fd);
        if(rc == -1) {
            m_logger->Log(LOGLEVEL_ERROR, 
                "wmsink: mq_close failed for (%d) (%d) (%s)", fd, errno, strerror(errno));
        }
    }
    rc = mq_unlink(m_myQueueName.c_str());
    if(rc == -1) {
        m_logger->Log(LOGLEVEL_ERROR, 
            "wmsink: mq_unlink failed for (%s) (%d) (%s)", 
            m_myQueueName.c_str(), errno, strerror(errno));
    }
    return rc;
}   

ssize_t waitableMessageSink::recv(waitable_message* Pmessage, size_t msg_len) 
{
    m_logger->Log(LOGLEVEL_INFO, "wmsi: gonna recv (%d)", sizeof(waitable_message));
    ssize_t amount = mq_receive(m_myFd, (char*) Pmessage, sizeof(waitable_message), nullptr);
    if(amount == -1) {
        m_logger->Log(LOGLEVEL_ERROR, "wmsi: mq_receive failed (%s)", strerror(errno));
    }
    return amount;
}

std::string waitableMessageSink::getName() 
{
    return m_myName;
}

int waitableMessageSink::getFd()
{
    return (int)m_myFd;
}

int waitableMessageSink::getId()
{
    return m_myId;
}

std::string waitableMessageSink::formQueueName(std::string name)
{
    std::string queueName = "/" + name;
    return queueName;
}


