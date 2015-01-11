#ifndef _MOVEMENT_COMPONENT_HPP_
#define _MOVEMENT_COMPONENT_HPP_

#include "Algebra.hpp"

#include "Component.hpp"

struct MovementComponent : public Component
{
	Vector3D velocity;
	Vector3D acceleration;
	double drag;

	Vector3D angular_velocity;
	Vector3D angular_acceleration;
	double angular_drag;
};

#endif