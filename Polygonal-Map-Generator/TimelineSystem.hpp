#ifndef _TIMELINE_SYSTEM_HPP_
#define _TIMELINE_SYSTEM_HPP_

#include "System.hpp"

class TimelineSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif
