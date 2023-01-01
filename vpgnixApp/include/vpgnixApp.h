#pragma once
#include <aspthread.h>
#include <corethread.h>
#include <memory>
#include <IRunnableApp.h>
#include <unistd.h>
#include <vpgnix.h>
#include <vpgnixlogger.h>
#include <waitable_event.h>

class vpgnixApp : public IRunnableApp {
  public:
    vpgnixApp(std::shared_ptr<ILogger>);
    vpgnixApp() = delete;
    ~vpgnixApp() = default;
    virtual void start() override;
    virtual void stop() override;

  private:
    std::shared_ptr<waitable_event> m_vpse;
    std::shared_ptr<vpgnix> m_vp;
    std::shared_ptr<waitable_event> m_ctse;
    std::shared_ptr<corethread> m_ct;
    std::shared_ptr<waitable_event> m_atse;
    std::shared_ptr<aspthread> m_at;
    std::shared_ptr<ILogger> m_logger;
};
