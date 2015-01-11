#ifndef _LIFE_SYSTEM_HPP_
#define _LIFE_SYSTEM_HPP_

#include "System.hpp"

class LifeSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif