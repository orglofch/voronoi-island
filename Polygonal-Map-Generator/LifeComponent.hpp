#ifndef _LIFE_COMPONENT_HPP_
#define _LIFE_COMPONENT_HPP_

#include "Component.hpp"

struct LifeComponent : public Component
{
	LifeComponent(double _life, double _decay_rate = 0)
	: life(_life), decay_rate(_decay_rate) {}

	double life;
	double decay_rate;
};

#endif