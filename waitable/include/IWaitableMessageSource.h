#pragma once
#include <IMessageSource.h>
#include <ISourceSinkIdentity.h>
#include <IMessageSourceSetup.h>

class IWaitableMessageSource : public IMessageSource, public IMessageSourceSetup, 
                               public ISourceSinkIdentity, public IWaitable {
public:
    virtual bool postThreadMessage(std::string, uint32_t, uint64_t, uint64_t) = 0;
    virtual bool postMessage(uint32_t, uint64_t, uint64_t) = 0;
    virtual ~IWaitableMessageSource() = default;
};
