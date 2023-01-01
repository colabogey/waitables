#pragma once
#include <unistd.h>
#include <waitable_message.h>
class IMessageSink {
public:
    virtual ssize_t recv(waitable_message*, size_t) = 0;
    virtual ~IMessageSink() = default;
};

