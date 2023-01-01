#include <nullLogger.h>

nullLogger::nullLogger() {
}

nullLogger::~nullLogger() {
}

void nullLogger::Uninitialize() {
}

void nullLogger::Initialize(std::shared_ptr<loggerConfig> lc) {
}

void nullLogger::Log(uint32_t nLevel, std::string sData, ...) {
}

void nullLogger::Log(uint32_t nLevel, cstring&) {
}

