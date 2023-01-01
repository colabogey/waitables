#include <threadNames.h>
#include <vpgnixlogger.h>
#include <waitable_messageSource.h>
#include <waitable_message.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

waitableMessageSource::waitableMessageSource(std::string name, 
                                             std::shared_ptr<ILogger> sp_logger)
{
    m_myName = name;
    m_myQueueName = formQueueName(name);
    m_myFd = -1;
    m_logger = sp_logger;
}

waitableMessageSource::~waitableMessageSource()
{
    for (const auto &[key, value] : m_destinationQueueMap) {
        if(value != -1) {
            close(value);
        }
    }
    int rc = mq_unlink(m_myQueueName.c_str());
    if(rc == -1) {
        m_logger->Log(LOGLEVEL_ERROR,
        "wmsource: mq_unlink failed for (%s) (%d) (%s)",
        m_myQueueName.c_str(), errno, strerror(errno));
    }
    m_destinationQueueMap.clear();
}

// threadId is not a queue name
bool waitableMessageSource::postThreadMessage(std::string threadId, 
            uint32_t message, uint64_t wParam, uint64_t lParam)
{
    waitable_message wm;
    wm.message = message;
    wm.wParam = wParam;
    wm.lParam = lParam;
    send(threadId, &wm, sizeof(waitable_message));
    return true;
}

bool waitableMessageSource::postMessage(uint32_t message, uint64_t wParam, uint64_t lParam)
{
    postThreadMessage(m_myName, message, wParam, lParam);
    return true;
}

void waitableMessageSource::initialize(const std::vector<std::string>& destinationThreads) 
{
    // should set queue attrs as an administrator via /proc ?
    m_queueAttr.mq_maxmsg = 1024;
    m_queueAttr.mq_msgsize = sizeof(waitable_message);

    m_myFd = open(m_myQueueName);
    m_destinationQueueMap.insert(std::pair<std::string, int>(m_myQueueName, m_myFd));
    for (const auto& s : destinationThreads) {
        std::string qName = formQueueName(s);
        int fd = open(qName);
        if(fd != -1)
        {
            m_destinationQueueMap.insert(std::pair<std::string, int>(qName, fd));
            m_logger->Log(LOGLEVEL_INFO, "wmso (init): add (%s) (%d)", qName.c_str(), fd);
        }
    }
}

int waitableMessageSource::open(const std::string& queueName) 
{
    mqd_t fd = mq_open(queueName.c_str(), 
                       O_CREAT | O_RDWR, 
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
                       &m_queueAttr);
    m_logger->Log(LOGLEVEL_INFO, "wmso (open): (%s) (%d)", queueName.c_str(), fd);
    if(fd == -1)
    {
        m_logger->Log(LOGLEVEL_ERROR, "wmsource: mq_open failed (%s)", strerror(errno));
    }
    return (int) fd;
}

int waitableMessageSource::close(int fd) 
{
        int rc = mq_close(fd);
        if(rc == -1) {
            m_logger->Log(LOGLEVEL_ERROR,
            "wmsource: mq_close failed for (%d) (%s)", fd, strerror(errno));
        }
        return rc;
}   

int waitableMessageSource::send(std::string threadId, 
                                waitable_message* Pmessage, size_t msg_len)
{
    int rc = -1;
    std::string qName = formQueueName(threadId);
    int queueFd = -1;
    auto result = m_destinationQueueMap.find(qName);
    if (result != m_destinationQueueMap.end()) {
        queueFd = result->second;
        m_logger->Log(LOGLEVEL_INFO, "wmso (send): len (%d)", msg_len);
        rc = mq_send(queueFd, (char*) Pmessage, msg_len, 0);
        if(rc == -1)
        {
            m_logger->Log(LOGLEVEL_ERROR, "wmso: send failed (%s)", strerror(errno));
        }
    } else {
        m_logger->Log(LOGLEVEL_ERROR, "wmso (send): (%s) is not in map", qName.c_str());
        dumpDestinationQueueMap();
    }
    return rc;
}

void waitableMessageSource::dumpDestinationQueueMap()
{
    for (const auto &[key, value] : m_destinationQueueMap) {
        m_logger->Log(LOGLEVEL_INFO, "wmso (dump): (%s) (%d)", key.c_str(), value);
    };
}

std::string waitableMessageSource::getName() 
{
    return m_myName;
}

int waitableMessageSource::getFd()
{
    return (int)m_myFd;
}

int waitableMessageSource::getId()
{
    return m_myId;
}

std::string waitableMessageSource::formQueueName(std::string name)
{
    std::string queueName = "/" + name;
    return queueName;
}
