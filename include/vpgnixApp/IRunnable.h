#pragma once

class IRunnable {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void run(int, std::string&) = 0;
    virtual ~IRunnable() = default;
};
