#ifndef _DAMAGE_COMPONENT_HPP_
#define _DAMAGE_COMPONENT_HPP_

#include "Component.hpp"

struct DamageComponent : public Component
{
	DamageComponent(double _damage) : damage(_damage) {}

	double damage;
};

#endif