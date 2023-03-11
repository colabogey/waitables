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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <watcher.h>
#include <thread>

#define EVENT_SIZE	sizeof (struct inotify_event)
#define BUF_LEN		1024 * ( EVENT_SIZE + 16 ) 

watcher::watcher(std::string dn, std::string fn)
: m_nfds(1)
, m_dn("")
, m_fn("")
, m_fd(-1)
, m_wd(-1)
, m_key(1)
, m_stop(false)
{
	m_dn = dn;
	m_fn = fn;
}

watcher::~watcher()
{
	m_t.join();
}

int32_t watcher::unwatch()
{
	m_stop = true;
	int32_t res = inotify_rm_watch(m_fd, m_wd);
    close(m_fd);
	return(res);
}

void watcher::add_watch()
{
	while((m_wd == -1) && !m_stop)
	{
		m_wd = inotify_add_watch(m_fd, m_dn.c_str(), IN_CREATE | IN_MODIFY);
		if((m_wd == -1) && (errno == ENOENT))
		{
			sleep(1);
		}
		else
		{
			break;
		}
	}
}

void watcher::performwatch()
{
	m_fd = inotify_init();
	add_watch();
	m_fds[0].fd = m_fd;
	m_fds[0].events = POLLIN;
	while(!m_stop)
	{
		if(wait_for_event())
		{
			 notify();
		}
	}
}

void watcher::watch()
{
	m_t = std::move(std::thread([=, this]() {watcher::performwatch(); return 1; }));
}

bool watcher::wait_for_event()
{
	char buffer[BUF_LEN];
	int32_t poll_num = poll(m_fds, m_nfds, 1000);
	bool watched_file_event = false;
	if(poll_num != 0)
	{
		int32_t i = 0;
		int32_t amt = read(m_fd, buffer, BUF_LEN);
		if(amt != -1)
		{
			while(i < amt)
			{
				struct inotify_event *ine = ( struct inotify_event * ) &buffer[i];
				if( ine->len > 0 )
				{
					std::string name = ine->name;
					int32_t match = name.compare(m_fn);
					if(match == 0)
					{
						watched_file_event = true;
					}
				}
				i += EVENT_SIZE + ine->len;
			}
		}
	}
	return watched_file_event;
}

int watcher::attach(IObserver *observer) {
	const std::lock_guard<std::mutex> lock(m_mutex_observer_map);
	m_observer_map.insert( std::pair<int32_t,IObserver*>(m_key, observer) );
	int32_t key_used = m_key;
	m_key++;
	return key_used;
}

int32_t watcher::detach(int32_t key) {
	const std::lock_guard<std::mutex> lock(m_mutex_observer_map);
    int32_t num_erased = m_observer_map.erase(key);
	return num_erased;
}

void watcher::notify() {
	const std::lock_guard<std::mutex> lock(m_mutex_observer_map);
	const std::string &message = m_fn;
	std::map<int32_t, IObserver*>::iterator it = m_observer_map.begin();
	for( ; it != m_observer_map.end(); ++it)
	{
	  it->second->Update(message);
	}
}

