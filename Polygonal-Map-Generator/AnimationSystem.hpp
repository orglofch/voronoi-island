#ifndef _ANIMATION_SYSTEM_HPP_
#define _ANIMATION_SYSTEM_HPP_

#include "System.hpp"

class AnimationSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif