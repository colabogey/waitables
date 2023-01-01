#pragma once
#include <string>
#include <IWaitable.h>

class waitable_event : public IWaitable {
  public:
    waitable_event() = default;
    virtual ~waitable_event();
    void create_event(int id);
    void set_event();
    void reset_event();
    virtual int getId() override;
    virtual int getFd() override;

  private:
    int m_fd;
    int m_id;
};
