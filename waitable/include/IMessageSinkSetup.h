#pragma once
#include <string>
#include <vector>

class IMessageSinkSetup {
public:
    virtual void initialize() = 0;
    virtual int open(const std::string&) = 0;
    virtual int close(int) = 0;
    virtual ~IMessageSinkSetup() = default;
};
