#ifndef _LIGHT_COMPONENT_HPP_
#define _LIGHT_COMPONENT_HPP_

#include "Colour.hpp"

#include "Component.hpp"

struct LightComponent : public Component
{
	LightComponent(const Colour &_colour, double _intensity)
	: colour(_colour), intensity(_intensity) {}

	Colour colour;
	double intensity;
};

#endif