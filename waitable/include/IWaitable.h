#pragma once
#include <string>

class IWaitable {
public:
    virtual int getFd() = 0;
    virtual int getId() = 0;
    virtual ~IWaitable() = default;
};
