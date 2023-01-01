#pragma once
#include <IMessageSink.h>
#include <ISourceSinkIdentity.h>
#include <IMessageSinkSetup.h>
#include <waitable_message.h>

class IWaitableMessageSink : public IMessageSink, public IMessageSinkSetup, 
                             public ISourceSinkIdentity, public IWaitable {
public:
    virtual bool peekMessage(waitable_message*, bool) = 0;
    virtual bool getMessage(waitable_message*) = 0;
    virtual ~IWaitableMessageSink() = default;
};
