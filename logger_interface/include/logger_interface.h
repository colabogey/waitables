#pragma once

#include <string>
#include "cstring.h"

class ILogger {
public:
	virtual ~ILogger() = default;
	virtual void Initialize(const std::string sLogDir) = 0;
	virtual void Initialize(std::shared_ptr<loggerConfig) = 0;
	virtual void Log(uint32_t nLevel, std::string sData, ...) = 0;
	virtual void Log(uint32_t nLevel, cstring&) = 0;
};
