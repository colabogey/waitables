#pragma once
#include <string>
#include <vector>
#include <map>
#include <mqueue.h>
#include <vpgnixlogger.h>
#include <IWaitable.h>
#include <IWaitableMessageSource.h>
#include <IMessageSource.h>
#include <ISourceSinkIdentity.h>

class waitableMessageSource : public IWaitableMessageSource {
  public:
    waitableMessageSource(std::string, std::shared_ptr<ILogger>);
    waitableMessageSource() = delete;
    virtual ~waitableMessageSource();

    virtual bool postThreadMessage(std::string, uint32_t, uint64_t, uint64_t) override;
    virtual bool postMessage(uint32_t, uint64_t, uint64_t) override;
    virtual int send(std::string, waitable_message*, size_t) override;
    virtual std::string getName() override;
    virtual int getFd() override;
    virtual int getId() override;
    virtual void initialize(const std::vector<std::string>&) override;

  private:
    virtual int open(const std::string&) override;
    virtual int close(int) override;
    std::string formQueueName(std::string name);
    void dumpDestinationQueueMap();

    mqd_t m_myFd;
    int m_myId;
    std::string m_myName;
    std::string m_myQueueName;
    std::map<std::string, int> m_destinationQueueMap;
    struct mq_attr m_queueAttr;
    std::shared_ptr<ILogger> m_logger;
};
