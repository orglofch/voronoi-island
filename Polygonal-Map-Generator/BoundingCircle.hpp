#ifndef _BOUNDING_CIRCLE_HPP_
#define _BOUNDING_CIRCLE_HPP_

#include "BoundingGeometry.hpp"
#include "Assertion.hpp"

class BoundingCircle : public BoundingGeometry
{
public:
	BoundingCircle() : BoundingGeometry(BOUNDING_GEOMETRY_CIRCLE) {}

	bool Contains(const Point3D &p);
	bool Intersects(const BoundingGeometry *other);
	bool IntersectsRay(const Ray &ray);
};

inline bool BoundingCircle::Contains(const Point3D &p)
{
	// TODO
	return false;
}

inline bool BoundingCircle::Intersects(const BoundingGeometry *other)
{
	switch (other->type()) {
	case BOUNDING_GEOMETRY_AABB:
	case BOUNDING_GEOMETRY_OBB:
	case BOUNDING_GEOMETRY_CIRCLE:
		break;
	}
	NOTREACHED();
	return false;
}

inline bool BoundingCircle::IntersectsRay(const Ray &ray)
{
	// TODO
	return false;
}

#endif