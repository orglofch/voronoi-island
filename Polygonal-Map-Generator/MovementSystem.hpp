#ifndef _MOVEMENT_SYSTEM_HPP_
#define _MOVEMENT_SYSTEM_HPP_

#include "System.hpp"

class MovementSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif