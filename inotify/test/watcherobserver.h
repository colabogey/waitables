
#include <string>
#include <observer.h>

class watcherobserver : public IObserver
{
public:
	watcherobserver() = default;
	virtual ~watcherobserver() = default;
	virtual void Update(const std::string&) override;
	void clearmessage() { m_the_update_message = "";};
	std::string getmessage() { return m_the_update_message ;};
private:
	std::string m_the_update_message;
};
