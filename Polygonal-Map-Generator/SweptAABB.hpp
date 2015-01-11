#ifndef _SWEPT_AABB_HPP_
#define _SWEPT_AABB_HPP_

#include "Algebra.hpp"
#include "AABB.hpp"

class SweptAABB
{
public:
	SweptAABB(const AABB &_aabb, const Vector3D &_movement)
		: aabb(_aabb), movement(_movement) {}

	bool Intersect(const AABB &aabb);

private:
	AABB aabb;
	Vector3D movement;
};

inline bool SweptAABB::Intersect(const AABB &aabb)
{
	// TODO
	return false;
}

#endif