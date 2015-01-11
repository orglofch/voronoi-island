#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

struct Component
{
	virtual ~Component() = 0;
};

inline Component::~Component() {}

#endif