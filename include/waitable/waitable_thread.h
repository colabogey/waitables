#pragma once
#include <functional>
#include <thread>
#include <string>
#include <IWaitable.h>

// TODO: make the string be const ?
//
typedef std::function<void(int, std::string&)> waitableThreadFunc_t;

class waitable_thread : public IWaitable {
  public:
    waitable_thread(std::string&, int, waitableThreadFunc_t f);
    waitable_thread() = delete;
    virtual ~waitable_thread();
    void func();
    void signalthreadexit();
    std::string getName();
    virtual int getFd() override;
    virtual int getId() override;

  private:
    int m_fd;
    int m_id;
    waitableThreadFunc_t m_f;
    std::thread m_t;
    std::string m_myThreadName;
};
