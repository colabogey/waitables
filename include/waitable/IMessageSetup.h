#pragma once

class IMessageSetup {
public:
    virtual int initialize() = 0;
    virtual int open(std::string) = 0;
    virtual int close(std::string) = 0;
    virtual ~IMessageSetup() = default;
};
