#pragma once
#include <atomic>
#include <thread>

class methodlistener
{
public:
	methodlistener();
	virtual ~methodlistener() = default;
	void listen();
	void reply_to_method_call(DBusMessage* msg, DBusConnection* conn);
	void stop();
	void setstop(bool b) {m_stop = b; };
	bool getstop() { return m_stop; };
	std::unique_ptr<std::thread> ml_thread_ptr; 
private:
	std::atomic<bool> m_stop;
};
