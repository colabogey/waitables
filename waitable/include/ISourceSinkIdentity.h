#pragma once
#include <string>

class ISourceSinkIdentity {
public:
    virtual std::string getName() = 0;
    virtual ~ISourceSinkIdentity() = default;
};
