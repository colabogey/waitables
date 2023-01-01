#pragma once

#include <observer.h>

class ISubject {
public:
	virtual ~ISubject() = default;
	virtual int32_t attach(IObserver *observer) = 0;
	virtual int32_t detach(int32_t) = 0;
	virtual void notify() = 0;
};
