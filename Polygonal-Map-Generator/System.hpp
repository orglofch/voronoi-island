#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "Message.hpp"

class System
{
public:
	virtual void Init(void) {};
	virtual void Update(const double deltaTime) = 0;

	virtual void HandleMessage(const Message *message) {};
};

#endif