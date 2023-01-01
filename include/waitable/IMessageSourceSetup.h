#pragma once
#include <string>
#include <vector>

class IMessageSourceSetup {
public:
    virtual void initialize(const std::vector<std::string>&) = 0;
    virtual int open(const std::string&) = 0;
    virtual int close(int) = 0;
    virtual ~IMessageSourceSetup() = default;
};
