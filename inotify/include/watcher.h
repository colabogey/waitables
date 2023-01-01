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


