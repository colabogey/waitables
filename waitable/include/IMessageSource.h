#pragma once
#include <unistd.h>
#include <waitable_message.h>

class IMessageSource {
public:
    virtual int send(std::string, waitable_message*, size_t) = 0;
    virtual ~IMessageSource() = default;
};

