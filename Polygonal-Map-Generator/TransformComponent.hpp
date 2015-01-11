#ifndef _TRANSFORM_COMPONENT_HPP_
#define _TRANSFORM_COMPONENT_HPP_

#include "Algebra.hpp"

#include "Component.hpp"

struct TransformComponent : public Component
{
	TransformComponent(const Point3D &pos) 
		: position(pos)
	{
		matrix = translation(pos.AsVector());
		inv_matrix = matrix.invert();
	}

	Matrix4x4 matrix;
	Matrix4x4 inv_matrix;

	Point3D position;
};

#endif