#ifndef _AI_SYSTEM_HPP_
#define _AI_SYSTEM_HPP_

#include "System.hpp"

class AISystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif