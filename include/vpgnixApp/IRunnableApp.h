#pragma once

class IRunnableApp {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~IRunnableApp() = default;
};
