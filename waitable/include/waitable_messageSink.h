#pragma once
#include <string>
#include <vector>
#include <mqueue.h>
#include <vpgnixlogger.h>
#include <IWaitable.h>
#include <IWaitableMessageSink.h>
#include <IMessageSink.h>
#include <ISourceSinkIdentity.h>
#include <IMessageSinkSetup.h>
#include <waitable_message.h>

class waitableMessageSink : public IWaitableMessageSink {
  public:
    waitableMessageSink(std::string, int, std::shared_ptr<ILogger>);
    waitableMessageSink() = delete;
    virtual ~waitableMessageSink();
    virtual bool peekMessage(waitable_message*, bool) override;
    virtual bool getMessage(waitable_message*) override;
    virtual ssize_t recv(waitable_message*, size_t) override;
    virtual std::string getName() override;
    virtual int getFd() override;
    virtual int getId() override;
    virtual void initialize() override;
  private:
    virtual int open(const std::string&) override;
    virtual int close(int) override;
    std::string formQueueName(std::string name);
    mqd_t m_myFd;
    int m_myId;
    std::string m_myName;
    std::string m_myQueueName;
    struct mq_attr m_queueAttr;
    std::shared_ptr<ILogger> m_logger;
};
