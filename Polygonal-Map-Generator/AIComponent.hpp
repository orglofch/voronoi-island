#ifndef _AI_COMPONENT_HPP_
#define _AI_COMPONENT_HPP_

#include "Component.hpp"

struct AIComponent : public Component
{
	enum AIType {
		AI_TYPE_AGGRESSIVE,
		AI_TYPE_PASSIVE,
	};

	AIComponent(AIType _type) : type(_type) {}

	AIType type;
};

#endif