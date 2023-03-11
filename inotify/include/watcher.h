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

#include <poll.h>
#include <unistd.h>

#include <string>
#include <map>
#include <sys/inotify.h>
#include <subject.h>
#include <atomic>
#include <thread>
#include <mutex>

class watcher : public ISubject
{
public:
	watcher(std::string dn, std::string fn);
	watcher() = default;
	virtual ~watcher();
	void watch();
	int unwatch();
	virtual int32_t attach(IObserver *observer) override ;
	virtual int32_t detach(int32_t) override ;
	virtual void notify() override ;

private:
	bool wait_for_event();
	void add_watch();
	void performwatch();

	nfds_t m_nfds;
	struct pollfd m_fds[1];
	std::map <int32_t, IObserver*> m_observer_map;

	std::string m_dn;
	std::string m_fn;
	int m_fd;
	int m_wd;
	int m_key;
	std::atomic<bool> m_stop;
	std::thread m_t;
	std::mutex m_mutex_observer_map; 
};


