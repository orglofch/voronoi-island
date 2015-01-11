#ifndef _DAMAGE_SYSTEM_HPP_
#define _DAMAGE_SYSTEM_HPP_

#include "System.hpp"

class DamageSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif