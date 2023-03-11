/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

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
